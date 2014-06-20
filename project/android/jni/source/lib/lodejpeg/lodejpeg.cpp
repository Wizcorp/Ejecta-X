#include "lodejpeg.h"
#include "jpeglib.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef LODEPNG_COMPILE_CPP
#include <fstream>
#endif /*LODEPNG_COMPILE_CPP*/

#define VERSION_STRING "20130318"
/*
This returns the description of a numerical error code in English. This is also
the documentation of all the error codes.
*/
const char* lodejpeg_error_text(unsigned code)
{
  switch(code)
  {
    case 0: return "no error, everything went ok";
  }
  return "unknown error code";
}

unsigned lodejpeg_load_file(unsigned char** out, size_t* outsize, const char* filename)
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


unsigned lodejpeg_decode_memory(unsigned char** out, unsigned* w, unsigned* h,
                               const unsigned char* in, size_t insize, unsigned bitdepth)
{
  unsigned error = 1;
  /* these are standard libjpeg structures for reading(decompression) */
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  /* libjpeg data structure for storing one row, that is, scanline of an image */
  JSAMPROW row_pointer[1] = {0};
  unsigned long location = 0;
  unsigned int i = 0;
  unsigned int j = 0;

  do 
  {
      /* here we set up the standard libjpeg error handler */
      cinfo.err = jpeg_std_error( &jerr );

      /* setup decompression process and source, then read JPEG header */
      jpeg_create_decompress( &cinfo );

      jpeg_mem_src( &cinfo, (unsigned char *) in, insize );

      /* reading the image header which contains image information */
      jpeg_read_header( &cinfo, true );

      // we only support RGB or grayscale
      if (cinfo.jpeg_color_space != JCS_RGB)
      {
          if (cinfo.jpeg_color_space == JCS_GRAYSCALE || cinfo.jpeg_color_space == JCS_YCbCr)
          {
              cinfo.out_color_space = JCS_RGB;
          }
      }
      else
      {
          break;
      }

      /* Start decompression jpeg here */
      jpeg_start_decompress( &cinfo );

      /* init image info */
      *w = cinfo.image_width;
      *h = cinfo.image_height;
      row_pointer[0] = new unsigned char[cinfo.output_width*cinfo.output_components];

      unsigned char * _out = new unsigned char[cinfo.output_width*cinfo.output_height*4];

      /* now actually read the jpeg into the raw buffer */
      /* read one scan line at a time */
      while( cinfo.output_scanline < cinfo.image_height )
      {
          jpeg_read_scanlines( &cinfo, row_pointer, 1 );
		  j=0;
          for( i=0; i<cinfo.image_width*4;i++) 
          {
			  if(i%4==3){
				  _out[location++] = 0xff;
			  }else {
				  _out[location++] = (row_pointer[0][j++]);
			  }
          }
      }
	  *out = _out;
      jpeg_finish_decompress( &cinfo );
      jpeg_destroy_decompress( &cinfo );
      /* wrap up decompression, destroy objects, free pointers and close open files */        
      error = 0;
  } while (0);

  if(row_pointer[0])delete row_pointer[0];
  return error;
}

unsigned lodejpeg_decode_file(unsigned char** out, unsigned* w, unsigned* h, const char* filename, unsigned bitdepth)
{
  unsigned char* buffer;
  size_t buffersize;
  unsigned error;
  error = lodejpeg_load_file(&buffer, &buffersize, filename);
  if(!error) error = lodejpeg_decode_memory(out, w, h, buffer, buffersize, bitdepth);
  free(buffer);
  return error;
}

unsigned lodejpeg_decode32_file(unsigned char** out, unsigned* w, unsigned* h, const char* filename)
{
  return lodejpeg_decode_file(out, w, h, filename, 8);
}