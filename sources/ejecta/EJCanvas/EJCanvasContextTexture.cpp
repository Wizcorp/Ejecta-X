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
	NSLOG("EJCanvasContextTexture create");
	m_texture = new EJTexture(width, height);

	bufferWidth = m_texture->realWidth;
	bufferHeight = m_texture->realHeight;

	EJCanvasContext::create();
#ifdef _WINDOWS
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->textureId, 0);
#else
	glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, m_texture->textureId, 0);
#endif
	prepare();

	glClearColor(1.0f, 0.0f, 0.0f, 0.5f);
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
	m_texture->release();
}

const char* EJCanvasContextTexture::getClassName() 
{
	return "EJCanvasContextTexture";
}

EJTexture* EJCanvasContextTexture::texture() 
{
	if( msaaNeedsResolving ) {	
		GLint boundFrameBuffer;

#ifdef _WINDOWS
		glGetIntegerv( GL_FRAMEBUFFER_BINDING, &boundFrameBuffer );
#else
		glGetIntegerv( GL_FRAMEBUFFER_BINDING_OES, &boundFrameBuffer );
#endif
		
		/*
			Android上不支持多重采样
		**/

		//Bind the MSAA and View frameBuffers and resolve
		// glBindFramebufferOES(GL_FRAMEBUFFER_OES, msaaFrameBuffer);
		// glBindFramebufferOES(GL_DRAW_FRAMEBUFFER_APPLE, viewFrameBuffer);
		// glResolveMultisampleFramebufferAPPLE();

#ifdef _WINDOWS
		glBindFramebuffer(GL_FRAMEBUFFER, boundFrameBuffer);
#else
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, boundFrameBuffer);
#endif
		
		msaaNeedsResolving = false;
	}
	
	return m_texture;
}

void EJCanvasContextTexture::prepare() {

	NSLOG("EJCanvasContextTexture prepare");
	
	//EJCanvasContext::prepare();
	msaaNeedsResolving = msaaEnabled;
}