/****************************************************************************

****************************************************************************/

#ifndef __EJ_SHARED_OPENGL_CONTEXT_H__
#define __EJ_SHARED_OPENGL_CONTEXT_H__

#include "EJGLProgram2D.h"
//TODO: #import "EJGLProgram2DRadialGradient.h"

#define EJ_OPENGL_VERTEX_BUFFER_SIZE 2048 //(32 * 1024) // 32kb on Ejecta iOS

class EJSharedOpenGLContext : public NSObject {
private:
	EJGLProgram2D *glProgram2DFlat;
	EJGLProgram2D *glProgram2DTexture;
	EJGLProgram2D *glProgram2DAlphaTexture;
	EJGLProgram2D *glProgram2DPattern;
	//TODO: EJGLProgram2DRadialGradient *glProgram2DRadialGradient;
	
	//EAGLContext *glContext2D;
	//EAGLSharegroup *glSharegroup;
	//NSMutableData *vertexBuffer;
	EJVertex vertexBuffer[EJ_OPENGL_VERTEX_BUFFER_SIZE];

	static EJSharedOpenGLContext *instance;

	EJSharedOpenGLContext();

public:
	~EJSharedOpenGLContext();

	EJGLProgram2D *getGlProgram2DFlat();
	EJGLProgram2D *getGlProgram2DTexture();
	EJGLProgram2D *getGlProgram2DAlphaTexture();
	EJGLProgram2D *getGlProgram2DPattern();
	//TODO: EJGLProgram2DRadialGradient *getGlProgram2DRadialGradient() const;

	//EAGLContext *glContext2D;
	//EAGLSharegroup *glSharegroup;
	EJVertex *getVertexBuffer();

	static EJSharedOpenGLContext *getInstance();

};

#endif // __EJ_SHARED_OPENGL_CONTEXT_H__
