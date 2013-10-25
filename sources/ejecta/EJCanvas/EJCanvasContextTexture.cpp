#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <GLES/gl.h>
#include <GLES/glext.h>
#endif
#include "EJCanvasContextTexture.h"

void EJCanvasContextTexture::create() 
{
	texture = new EJTexture(width, height);

	bufferWidth = texture->realWidth;
	bufferHeight = texture->realHeight;

	EJCanvasContext::create();
#ifdef _WINDOWS
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->textureId, 0);
#else
	glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, texture->textureId, 0);
#endif
	prepare();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

EJCanvasContextTexture::EJCanvasContextTexture() 
{

}

EJCanvasContextTexture::EJCanvasContextTexture(short widthp, short heightp) : EJCanvasContext( widthp, heightp)
{

}

EJCanvasContextTexture::~EJCanvasContextTexture() 
{
	texture->release();
}

const char* EJCanvasContextTexture::getClassName() 
{
	return "EJCanvasContextTexture";
}

EJTexture* EJCanvasContextTexture::getTexture() 
{
	if( msaaNeedsResolving ) {	
		GLint boundFrameBuffer;

#ifdef _WINDOWS
		glGetIntegerv( GL_FRAMEBUFFER_BINDING, &boundFrameBuffer );
		
		//not support  Android MSAA

		//Bind the MSAA and View frameBuffers and resolve
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, msaaFrameBuffer);
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, viewFrameBuffer);
		//glResolveMultisampleFramebufferAPPLE();
#else
		glGetIntegerv( GL_FRAMEBUFFER_BINDING_OES, &boundFrameBuffer );
#endif

#ifdef _WINDOWS
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, boundFrameBuffer);
#else
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, boundFrameBuffer);
#endif
		
		msaaNeedsResolving = false;
	}
	
	return texture;
}

void EJCanvasContextTexture::prepare() 
{
	EJCanvasContext::prepare();
	msaaNeedsResolving = msaaEnabled;
}