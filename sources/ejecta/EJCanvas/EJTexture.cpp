#include <math.h>
#include "EJTexture.h"
#include "../lodepng/lodepng.h"
#include "../lodejpeg/lodejpeg.h"


// Textures check this global filter state when binding
static GLint EJTextureGlobalFilter = GL_LINEAR;

bool EJTexture::smoothScaling() {
	return (EJTextureGlobalFilter == GL_LINEAR);
}

void EJTexture::setSmoothScaling(bool smoothScaling) {
	EJTextureGlobalFilter = smoothScaling ? GL_LINEAR : GL_NEAREST;
}

EJTexture::EJTexture() : textureId(0), width(0), height(0), realWidth(0), realHeight(0) {
}

EJTexture::EJTexture(NSString * path) : textureId(0), width(0), height(0), realWidth(0), realHeight(0) {
	// For loading on the main thread (blocking)
	contentScale = 1;
	path->retain();
	fullPath = path;
	GLubyte * pixels = loadPixelsFromPath(path);
	createTextureWithPixels(pixels, GL_RGBA);
	free(pixels);
}

EJTexture::EJTexture(NSString * path, NSObject* sharegroup) : textureId(0), width(0), height(0), realWidth(0), realHeight(0) {
	//For loading in a background thread

	// If we're running on the main thread for some reason, take care
	// to not corrupt the current EAGLContext
	bool isMainThread = true;

	contentScale = 1;
	path->retain();
	fullPath = path;
	GLubyte * pixels = loadPixelsFromPath(path);

	if( pixels ) {

		
		createTextureWithPixels(pixels, GL_RGBA);

		if( !isMainThread ) {
			glFlush();
			//[context release];
		}

		free(pixels);
	}
}

EJTexture::EJTexture(int widthp, int heightp, GLenum formatp) : textureId(0), width(0), height(0), realWidth(0), realHeight(0) {
	// Create an empty texture
	contentScale = 1;
	NSString* empty = NSStringMake("[Empty]");
	empty->retain();
	fullPath = empty;
	setWidthAndHeight(widthp, heightp);
	createTextureWithPixels(NULL, formatp);
}

EJTexture::EJTexture(int widthp, int heightp) : textureId(0), width(0), height(0), realWidth(0), realHeight(0) {
	// Create an empty RGBA texture
	//EJTexture(widthp, heightp, GL_RGBA);
	contentScale = 1;
	NSString* empty = NSStringMake("[Empty]");
	empty->retain();
	fullPath = empty;
	setWidthAndHeight(widthp, heightp);
	createTextureWithPixels(NULL, GL_RGBA);
}

EJTexture::EJTexture(int widthp, int heightp, GLubyte * pixels) : textureId(0), width(0), height(0), realWidth(0), realHeight(0) {
	// Creates a texture with the given pixels

	contentScale = 1;
	NSString* empty = NSStringMake("[From Pixels]");
	empty->retain();
	fullPath = empty;
	setWidthAndHeight(widthp, heightp);
	
	if( width != realWidth || height != realHeight ) {
		GLubyte * pixelsPow2 = (GLubyte *)calloc( realWidth * realHeight * 4, sizeof(GLubyte) );
		for( int y = 0; y < height; y++ ) {
			memcpy( &pixelsPow2[y*realWidth*4], &pixels[y*width*4], width * 4 );
		}
		createTextureWithPixels(pixelsPow2, GL_RGBA);
		free(pixelsPow2);
	}
	else {
		createTextureWithPixels(pixels, GL_RGBA);
	}
}

EJTexture::~EJTexture() {
	fullPath->release();
	glDeleteTextures( 1, &textureId );
}

void EJTexture::setWidthAndHeight(int widthp, int heightp) {
	width = widthp;
	height = heightp;

	// The internal (real) size of the texture needs to be a power of two
	realWidth = pow(2, ceil(log10((double)width)/log10(2.0)));
	realHeight = pow(2, ceil(log10((double)height)/log10(2.0)));
}

void EJTexture::createTextureWithPixels(GLubyte * pixels, GLenum formatp) {
	// Release previous texture if we had one
	if (textureId) {
		glDeleteTextures(1, &textureId);
		textureId = 0;
	}

	GLint maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

	if (realWidth > maxTextureSize || realHeight > maxTextureSize) {
		NSLOG("Warning: Image %s larger than MAX_TEXTURE_SIZE (%d)", fullPath->getCString(), maxTextureSize);
	}
	format = formatp;

	bool wasEnabled = glIsEnabled(GL_TEXTURE_2D);
	int boundTexture = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

	glEnable (GL_TEXTURE_2D);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, realWidth, realHeight, 0, format,
			GL_UNSIGNED_BYTE, pixels);

	setFilter(EJTextureGlobalFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, boundTexture);
	if (!wasEnabled) {
		glDisable(GL_TEXTURE_2D);
	}
}

void EJTexture::setFilter(GLint filter) {
	textureFilter = filter;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilter);
}

void EJTexture::updateTextureWithPixels(GLubyte * pixels, int x, int y,
		int subWidth, int subHeight) {
	if (!textureId) {
		NSLOG("No texture to update. Call createTexture... first");
		return;
	}

	bool wasEnabled = glIsEnabled(GL_TEXTURE_2D);
	int boundTexture = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, subWidth, subHeight, format,
			GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, boundTexture);
	if (!wasEnabled) {
		glDisable (GL_TEXTURE_2D);
	}
}

GLubyte * EJTexture::loadPixelsFromPath(NSString * path) {
	
	// All CGImage functions return pixels with premultiplied alpha and there's no
	// way to opt-out - thanks Apple, awesome idea.
	// So, for PNG images we use the lodepng library instead.
	
	return (std::string(path->getCString()).find(".png") == std::string::npos)?
		loadPixelsWithCGImageFromPath(path):loadPixelsWithLodePNGFromPath(path);
}

GLubyte * EJTexture::loadPixelsWithCGImageFromPath(NSString * path) {
	unsigned int w, h;
	unsigned char * origPixels = NULL;
	unsigned int error = lodejpeg_decode32_file(&origPixels, &w, &h, path->getCString());
	if( error ) {
		NSLOG("Error Loading image %s - %u: %s", path->getCString(), error, lodejpeg_error_text(error));
		return origPixels;
	}

	setWidthAndHeight(w, h);

	// If the image is already in the correct (power of 2) size, just return
	// the original pixels unmodified

	if( width == realWidth && height == realHeight ) {
		return origPixels;
	}

	// Copy the original pixels into the upper left corner of a larger
	// (power of 2) pixel buffer, free the original pixels and return
	// the larger buffer
	else {
		GLubyte * pixels = (GLubyte *)calloc( realWidth * realHeight * 4, sizeof(GLubyte) );

		for( int y = 0; y < height; y++ ) {
			memcpy( &pixels[y*realWidth*4], &origPixels[y*width*4], width*4 );
		}

		free( origPixels );
		return pixels;
	}
}

GLubyte * EJTexture::loadPixelsWithLodePNGFromPath(NSString * path) {
	unsigned int w, h;
	unsigned char * origPixels = NULL;
	unsigned int error = lodepng_decode32_file(&origPixels, &w, &h, path->getCString());
	if( error ) {
		NSLOG("Error Loading image %s - %u: %s", path->getCString(), error, lodepng_error_text(error));
		return origPixels;
	}

	setWidthAndHeight(w, h);

	// If the image is already in the correct (power of 2) size, just return
	// the original pixels unmodified

	if( width == realWidth && height == realHeight ) {
		return origPixels;
	}

	// Copy the original pixels into the upper left corner of a larger
	// (power of 2) pixel buffer, free the original pixels and return
	// the larger buffer
	else {
		GLubyte * pixels = (GLubyte *)calloc( realWidth * realHeight * 4, sizeof(GLubyte) );

		for( int y = 0; y < height; y++ ) {
			memcpy( &pixels[y*realWidth*4], &origPixels[y*width*4], width*4 );
		}

		free( origPixels );
		return pixels;
	}
}

void EJTexture::bind() {
	glBindTexture(GL_TEXTURE_2D, textureId);
	if (EJTextureGlobalFilter != textureFilter) {
		setFilter(EJTextureGlobalFilter);
	}
}