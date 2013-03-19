#ifndef __EJTEXTURE_H__
#define __EJTEXTURE_H__

#ifdef _WINDOWS
#include <gl/glew.h>
#include <gl/wglew.h>
#include <windows.h>
#include <tchar.h>
#include <gl//gl.h>
#include <gl/glext.h>
#else
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif
#include "../EJCocoa/NSString.h"

class EJTexture : public NSObject {

	NSString * fullPath;
	GLenum format;
	GLint textureFilter;

	void setFilter(GLint filter);

public:

	float contentScale;
	GLuint textureId;
	short width, height, realWidth, realHeight;

	EJTexture();
	EJTexture(NSString * path);
	EJTexture(NSString * path, NSObject* sharegroup);
	EJTexture(int widthp, int heightp, GLenum format);
	EJTexture(int widthp, int heightp);
	EJTexture(int widthp, int heightp, GLubyte * pixels);
	~EJTexture();

	void setWidthAndHeight(int width, int height);
	void createTextureWithPixels(GLubyte * pixels, GLenum format);
	void updateTextureWithPixels(GLubyte * pixels, int atx, int aty,
			int subWidth, int subHeight);

	GLubyte * loadPixelsFromPath(NSString * path);
	GLubyte * loadPixelsWithCGImageFromPath(NSString * path);
	GLubyte * loadPixelsWithLodePNGFromPath(NSString * path);

	void bind();

	static bool smoothScaling();
	static void setSmoothScaling(bool smoothScaling);
};

#endif // __EJTEXTURE_H__