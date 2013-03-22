#include "lodefreetype.h"

extern "C"
{
#include "ft2build.h"
#include FT_OUTLINE_H
#include FT_MODULE_H
}

static unsigned is_init_freetype = 0;
static FT_Library s_freetype;

/*
This returns the description of a numerical error code in English. This is also
the documentation of all the error codes.
*/
const char* lodefreetype_error_text(unsigned code)
{
  switch(code)
  {
    case 0: return "no error, everything went ok";
  }
  return "freetype error code ...";
}

unsigned lodefreetype_load_file(unsigned char** out, size_t* outsize, const char* filename)
{
  FILE* file;
  long size;

  /*provide some proper output values if error will happen*/
  *out = 0;
  *outsize = 0;

  file = fopen(filename, "rb");
  if(!file) return 78;

  /*get filesize:*/
  fseek(file , 0 , SEEK_END);
  size = ftell(file);
  rewind(file);

  /*read contents of the file into the vector*/
  *outsize = 0;
  *out = (unsigned char*)malloc((size_t)size);
  if(size && (*out)) (*outsize) = fread(*out, 1, (size_t)size, file);

  fclose(file);
  if(!(*out) && size) return 83; /*the above malloc failed*/
  return 0;
}


unsigned lodefreetype_decode_memory(void** data , unsigned* w, unsigned* h, const unsigned char* in, size_t data_size )
{
    FT_Face face0;
    unsigned error = FT_New_Memory_Face(s_freetype, static_cast<const FT_Byte*>(in), data_size, 0, &face0);
    
	unsigned long face_num = face0->num_faces;
	void* font_info = malloc(sizeof(FT_Face) * face_num);
	if(error)return error;

    FT_Face* faces = reinterpret_cast<FT_Face*>(font_info);
    faces[0] = face0;

    for (unsigned i = 1; i < face_num; i++)
    {
        error = FT_New_Memory_Face(s_freetype, static_cast<const FT_Byte*>(in), data_size, i, &faces[i]);
        if(error)return error;
    }
	*data = font_info;
    return error;
}


unsigned lodefreetype_decode32_file(void** out, unsigned* w, unsigned* h, const char* filename)
{
  unsigned char* buffer;
  size_t buffersize;
  unsigned error;
  create_freetype_font();
  error = lodefreetype_load_file(&buffer, &buffersize, filename);
  if(!error) error = lodefreetype_decode_memory(out, w, h, buffer, buffersize);
  return error;
}


unsigned create_freetype_font()
{
    if(!s_freetype) {
		unsigned error = FT_Init_FreeType( &s_freetype);
		if(error)return error;
    }
	return 0;
}


unsigned delete_freetype_font(unsigned char* font_info)
{
    unsigned error = 0;
    FT_Face* faces = reinterpret_cast<FT_Face*>(font_info);
    unsigned long face_num = faces[0]->num_faces;

    for (unsigned i = 0; i < face_num; i++)
    {
        error = FT_Done_Face(faces[i]);
        if(error)return error;
    }

    delete font_info;

    error = FT_Done_Library(s_freetype);
    return error;
}

void charToWchar(wchar_t* buf, size_t buf_size, const char* str)
{
	if (!buf || buf_size == 0 || !str)
	{
		return;
	}

	for (int i = 0; i < buf_size; i++)
	{
		char c = str[i];

		if (!c)
		{
			buf[i] = L'\0';
			break;
		}

		buf[i] = c;
	}

	buf[buf_size - 1] = L'\0';
}


struct RasterInfo
{
	void* image;
	unsigned width;
	unsigned int pen_x, pen_y;
};


static void rasterSpanFunc(int y, int count, const FT_Span* spans, void* user)
{
	RasterInfo* raster_info = static_cast<RasterInfo*>(user);

	y = raster_info->pen_y - (y + 1);

	char* dest_start = static_cast<char*>(raster_info->image) + raster_info->width * y + raster_info->pen_x;

	for (int i = 0; i < count; i++)
	{
		const FT_Span* span = &spans[i];
		char* dest = dest_start + span->x;
		unsigned len = span->len;
		char c = span->coverage;

		for (int j = 0; j < len; j++)
		{
			*dest = c;

			dest++;
		}
	}
}

unsigned int draw_freetype_font(void* image, unsigned* image_width, unsigned* image_height, void* font_info, unsigned long font_index, size_t font_size, unsigned int x, unsigned int y, const char* str)
{
	int len = strlen(str);
	wchar_t* wstr = new wchar_t[len];
	charToWchar(wstr, len, str);

	//std::string str1 = str;
	//std::wstring str2(L"");//wstring not support in android...
	//str2.assign(str1.begin(), str1.end());
	//const wchar_t * wstr = str2.c_str();

	if(!font_info)return -1;
	FT_Face face = *(reinterpret_cast<FT_Face*>(font_info) + font_index);

    FT_Size_RequestRec size_req;
    size_req.type = FT_SIZE_REQUEST_TYPE_CELL;
    size_req.width = 0;
    size_req.height = font_size << 6;
    size_req.horiResolution = 0;
    size_req.vertResolution = 0;

    if (FT_Request_Size(face, &size_req))
    {
        return -1;
    }

	if (image)
	{
		unsigned int pen_x = x;
		unsigned int pen_y = y + static_cast<unsigned int>(face->ascender * face->size->metrics.y_ppem / face->units_per_EM);

		RasterInfo raster_info;
		raster_info.image = image;
		raster_info.width = *image_width;

		FT_Raster_Params raster_params = {};
		raster_params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_CLIP;
		raster_params.gray_spans = static_cast<FT_Raster_Span_Func>(rasterSpanFunc);
		raster_params.user = &raster_info;
		raster_params.clip_box.yMin = pen_y - *image_height;
		raster_params.clip_box.yMax = pen_y;

		FT_GlyphSlot glyph_slot = face->glyph;

		for ( ; *str != L'\0'; str++)
		{
			if (FT_Load_Char(face, *str, FT_LOAD_DEFAULT | FT_LOAD_NO_BITMAP))
			{
				return -1;
			}

			if (glyph_slot->format != FT_GLYPH_FORMAT_OUTLINE)
			{
				return -1;
			}

			raster_params.clip_box.xMin = -pen_x;
			raster_params.clip_box.xMax = *image_width - pen_x;

			raster_info.pen_x = pen_x;
			raster_info.pen_y = pen_y;

			if (FT_Outline_Render(s_freetype, &glyph_slot->outline, &raster_params))
			{
				return -1;
			}

			pen_x += glyph_slot->advance.x >> 6;
		}

		return pen_x - x;
	}
	else
	{
		unsigned int pen_x = 0;
		FT_GlyphSlot glyph_slot = face->glyph;

		for ( ; *str != L'\0'; str++)
		{
			if (FT_Load_Char(face, *str, FT_LOAD_RENDER ))
			{
				return -1;
			}

			pen_x += glyph_slot->advance.x >> 6;
		}
		*image_width = (unsigned int)(glyph_slot->bitmap.width);
		*image_height = (unsigned int)(glyph_slot->bitmap.rows);

		return pen_x;
	}

}