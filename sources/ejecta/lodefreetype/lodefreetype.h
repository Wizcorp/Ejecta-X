#ifndef __LODEFREETYPE_H_
#define __LODEFREETYPE_H_

#ifdef __cplusplus
#include <vector>
#include <string>
#endif /*__cplusplus*/

/*Returns an English description of the numerical error code.*/
const char* lodefreetype_error_text(unsigned code);

/*
Load a file from disk into buffer. The function allocates the out buffer, and
after usage you should free it.
out: output parameter, contains pointer to loaded buffer.
outsize: output parameter, size of the allocated out buffer
filename: the path to the file to load
return value: error code (0 means ok)
*/
unsigned lodefreetype_load_file(unsigned char** out, size_t* outsize, const char* filename);

unsigned lodefreetype_decode_memory(void** out, unsigned* w, unsigned* h,
              const unsigned char* in, size_t insize);

/*Same as lodefreetype_decode32_file, but always decodes to 32-bit RGB raw image.*/
unsigned lodefreetype_decode32_file(void** out, unsigned* w, unsigned* h,
                               const char* filename);

unsigned create_freetype_font();
unsigned delete_freetype_font(unsigned char* font_info);
unsigned draw_freetype_font(char** image, unsigned* w, unsigned* h, void* font_info, unsigned long font_index, size_t font_size, unsigned int x, unsigned int y, const char* str);

#endif //__LODEFREETYPE_H_
