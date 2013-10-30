#include "EJSharedOpenGLContext.h"

EJSharedOpenGLContext *EJSharedOpenGLContext::instance = NULL;

EJSharedOpenGLContext::EJSharedOpenGLContext():
	glProgram2DFlat(NULL),
	glProgram2DTexture(NULL),
	glProgram2DAlphaTexture(NULL),
	glProgram2DPattern(NULL)
	//TODO: glProgram2DRadialGradient(NULL),
{
}

EJSharedOpenGLContext::~EJSharedOpenGLContext() {
	instance = NULL;

	if(glProgram2DFlat) {
		glProgram2DFlat->release();
		glProgram2DFlat = NULL;
	}
	if(glProgram2DTexture) {
		glProgram2DTexture->release();
		glProgram2DTexture = NULL;
	}
	if(glProgram2DAlphaTexture) {
		glProgram2DAlphaTexture->release();
		glProgram2DAlphaTexture = NULL;
	}
	if(glProgram2DPattern) {
		glProgram2DPattern->release();
		glProgram2DPattern = NULL;
	}
	/*TODO: if(glProgram2DRadialGradient) {
		glProgram2DRadialGradient->release();
		glProgram2DRadialGradient = NULL;
	}*/

}

EJSharedOpenGLContext *EJSharedOpenGLContext::getInstance() {
	if(instance == NULL) {
		instance = new EJSharedOpenGLContext();
	}
	return instance;
}

EJVertex *EJSharedOpenGLContext::getVertexBuffer() {
	return &vertexBuffer[0];
}

#define EJ_GL_PROGRAM_GETTER(TYPE, NAME, VERTEX_SHADER, FRAGMENT_SHADER) \
	TYPE *EJSharedOpenGLContext::getGlProgram2D##NAME() { \
		if(glProgram2D##NAME == NULL) { \
			glProgram2D##NAME = new TYPE(); \
			if(glProgram2D##NAME != NULL) { \
				bool shaderInitialization = glProgram2D##NAME->initWithVertexShader(NSString::createWithFormat("%s.vsh", #VERTEX_SHADER), NSString::createWithFormat("%s.fsh", #FRAGMENT_SHADER)); \
				if(!shaderInitialization) { \
					delete glProgram2D##NAME; \
					glProgram2D##NAME = NULL; \
					return NULL; \
				} \
			} \
		} \
		return glProgram2D##NAME; \
	}

EJ_GL_PROGRAM_GETTER(EJGLProgram2D, Flat, Vertex, Flat);
EJ_GL_PROGRAM_GETTER(EJGLProgram2D, Texture, Vertex, Texture);
EJ_GL_PROGRAM_GETTER(EJGLProgram2D, AlphaTexture, Vertex, AlphaTexture);
EJ_GL_PROGRAM_GETTER(EJGLProgram2D, Pattern, Vertex, Pattern);
//TODO: EJ_GL_PROGRAM_GETTER(EJGLProgram2DRadialGradient, RadialGradient, Vertex, RadialGradient);

#undef EJ_GL_PROGRAM_GETTER