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

#include "EJTextureStorage.h"

typedef enum {
	kEJTextureOwningContextCanvas2D,
	kEJTextureOwningContextWebGL
} EJTextureOwningContext;

class EJTexture : public NSObject {

	bool cached;
	GLuint fbo;
	EJTextureStorage *textureStorage;
	
	EJTextureOwningContext owningContext;
	EJTextureParams params;
	//NSBlockOperation *loadCallback;

	NSString * fullPath;
	GLint textureFilter;

	void setFilter(GLint filter);

public:

	float contentScale;
	GLenum format;
	GLuint textureId;
	bool isDynamic;
	unsigned char *pixels;
	short width, height, realWidth, realHeight;

	EJTexture();
	EJTexture(NSString * path);
	EJTexture(NSString * path, NSObject* queue, NSObject* callback);
	EJTexture(int widthp, int heightp);
	EJTexture(int widthp, int heightp, GLenum format);
	EJTexture(int widthp, int heightp, GLubyte * pixels);
	EJTexture(int widthp, int heightp, GLuint * fbop, float contentScalep);
	~EJTexture();

	static EJTexture* cachedTexture(NSString * path, NSObject* queue, NSObject* callback);

	void setWidthAndHeight(int width, int height);
	void ensureMutable(bool keepPixels, GLenum target);
	void createTextureWithPixels(EJTexture * other);
	void createTextureWithPixels(GLubyte * pixels, GLenum format);
	void createTextureWithPixels(GLubyte * pixels, GLenum format, GLenum target);
	void updateTextureWithPixels(GLubyte * pixels, int atx, int aty, int subWidth, int subHeight);

	GLubyte * loadPixelsFromPath(NSString * path);
	GLubyte * loadPixelsWithCGImageFromPath(NSString * path);
	GLubyte * loadPixelsWithLodePNGFromPath(NSString * path);

	GLint getParam(GLenum pname);
	void setParam(GLenum pname, GLenum param);

	void bindWithFilter(GLenum filter);
	void bindToTarget(GLenum target);

	static bool smoothScaling();
	static void setSmoothScaling(bool smoothScaling);
	static void premultiplyPixels(const GLubyte * inPixels, GLubyte * outPixels, int byteLength, GLenum format);
	static void unPremultiplyPixels(const GLubyte * inPixels, GLubyte * outPixels, int byteLength, GLenum format);
	static void flipPixelsY(GLubyte * pixels, int bytesPerRow, int rows);
};

#endif // __EJTEXTURE_H__
