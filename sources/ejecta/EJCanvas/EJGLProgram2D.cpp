#include "EJGLProgram2D.h"

#include "../EJApp.h"

EJGLProgram2D::EJGLProgram2D(): program(0), screen(0) {

}

EJGLProgram2D::~EJGLProgram2D() {
	if(program) {
		glDeleteProgram(program);
	}
}

bool EJGLProgram2D::initWithVertexShader(NSString *vertexShaderFile, NSString *fragmentShaderFile) {
	program = glCreateProgram();
	GLuint vertexShader = compileShaderFile(vertexShaderFile, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShaderFile(fragmentShaderFile, GL_FRAGMENT_SHADER);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	
	bindAttributeLocations();
	
	linkProgram(program);

	getUniforms();
	
	glDetachShader(program, vertexShader);
	glDeleteShader(vertexShader);
	
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);

	return true;
}

void EJGLProgram2D::bindAttributeLocations() {
	glBindAttribLocation(program, kEJGLProgram2DAttributePos, "pos");
	glBindAttribLocation(program, kEJGLProgram2DAttributeUV, "uv");
	glBindAttribLocation(program, kEJGLProgram2DAttributeColor, "color");
}

void EJGLProgram2D::getUniforms() {
	screen = glGetUniformLocation(program, "screen");
}

GLint EJGLProgram2D::compileShaderFile(NSString *file, GLenum type) {
	NSString *path = EJApp::instance()->pathForResource(file);
	NSString *source = NSString::createWithContentsOfFile(path->getCString());
	if(!source) {
		NSLOG("Failed to load shader file %s", file->getCString());
		return 0;
	}

	return compileShaderSource(source, type);
}

GLint EJGLProgram2D::compileShaderSource(NSString *source, GLenum type) {
	const GLchar *glsource = (GLchar *)source->getCString();
	
	GLint shader = glCreateShader(type);
	glShaderSource(shader, 1, &glsource, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == 0) {
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		if(logLength > 0) {
			GLchar *log = (GLchar *)malloc(logLength);
			glGetShaderInfoLog(shader, logLength, &logLength, log);
			NSLOG("Shader compile log:\n%s", log);
			free(log);
		}
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void EJGLProgram2D::linkProgram(GLuint program) {
	GLint status;
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status == 0) {
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		if(logLength > 0) {
			GLchar *log = (GLchar *)malloc(logLength);
			glGetProgramInfoLog(program, logLength, &logLength, log);
			NSLOG("Program link log:\n%s", log);
			free(log);
		}
	}
}
