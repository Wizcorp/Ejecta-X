#ifndef __EJ_TEXTURE_STORAGE_H__
#define __EJ_TEXTURE_STORAGE_H__

// #import <Foundation/Foundation.h>
// #import <OpenGLES/EAGL.h>
// #import <OpenGLES/ES1/gl.h>
// #import <OpenGLES/ES1/glext.h>

typedef enum {
	kEJTextureParamMinFilter,
	kEJTextureParamMagFilter,
	kEJTextureParamWrapS,
	kEJTextureParamWrapT,
	kEJTextureParamLast
} EJTextureParam;

typedef EJTextureParam EJTextureParams[kEJTextureParamLast];


class EJTextureStorage : public NSObject
{

	EJTextureParams params;

public:

	GLuint textureId;
	BOOL immutable;

	EJTextureStorage();
	EJTextureStorage(bool immut);
	~EJTextureStorage();

	void bindToTarget(GLenum target, EJTextureParam * newParams);

};

#endif // __EJ_TEXTURE_STORAGE_H__