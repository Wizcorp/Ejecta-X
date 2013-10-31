#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif
#include "../EJApp.h"
#include "EJCanvasContext.h"

//EJVertex CanvasVertexBuffer[EJ_CANVAS_VERTEX_BUFFER_SIZE];

EJCanvasContext::EJCanvasContext() :
	viewFrameBuffer(0),
	viewRenderBuffer(0),
	msaaFrameBuffer(0),
	msaaRenderBuffer(0),
	stencilBuffer(0),
	vertexBufferSize(0),
	vertexBufferIndex(0),
	sharedGLContext(NULL),
	vertexBuffer(NULL),
	upsideDown(false),
	currentProgram(NULL)
{
}

//返回类名
const char* EJCanvasContext::getClassName() {
	return "EJCanvasContext";
}

EJCanvasContext::EJCanvasContext(short widthp, short heightp) :
	viewFrameBuffer(0),
	viewRenderBuffer(0),
	msaaFrameBuffer(0),
	msaaRenderBuffer(0),
	stencilBuffer(0),
	vertexBufferIndex(0),
	upsideDown(false),
	currentProgram(NULL)
{
	sharedGLContext = EJApp::instance()->getOpenGLContext();
	if(sharedGLContext != NULL) {
		vertexBuffer = sharedGLContext->getVertexBuffer();
		vertexBufferSize = EJ_OPENGL_VERTEX_BUFFER_SIZE;
	} else {
		vertexBuffer = NULL;
		vertexBufferSize = 0;
	}

	memset(stateStack, 0, sizeof(stateStack));
	stateIndex = 0;
	state = &stateStack[stateIndex];
	state->globalAlpha = 1;
	state->globalCompositeOperation = kEJCompositeOperationSourceOver;
	state->transform = CGAffineTransformIdentity;
	state->lineWidth = 1;
	state->lineCap = kEJLineCapButt;
	state->lineJoin = kEJLineJoinMiter;
	state->miterLimit = 10;
	state->textBaseline = kEJTextBaselineAlphabetic;
	state->textAlign = kEJTextAlignStart;
	//state->font = [[UIFont fontWithName:@"Helvetica" size:10] retain];
	state->font = new UIFont(NSStringMake("simsun.ttc"),32);
	state->clipPath = NULL;
	
	setScreenSize(widthp, heightp);
	
	path = new EJPath();
	backingStoreRatio = 1;
	
	fontCache = new NSCache();
	fontCache->setCountLimit(8);
	
	imageSmoothingEnabled = true;
	msaaEnabled = false;
	msaaSamples = 2;
}

EJCanvasContext::~EJCanvasContext()
{
	fontCache->release();
	
	// Release all fonts and clip paths from the stack
	for( int i = 0; i < stateIndex + 1; i++ ) {
		stateStack[i].font->release();
		//stateStack[i].clipPath->release();
	}

#ifdef _WINDOWS
	if( viewFrameBuffer ) { glDeleteFramebuffers( 1, &viewFrameBuffer); }
	if( viewRenderBuffer ) { glDeleteRenderbuffers(1, &viewRenderBuffer); }
	if( msaaFrameBuffer ) {	glDeleteFramebuffers( 1, &msaaFrameBuffer); }
	if( msaaRenderBuffer ) { glDeleteRenderbuffers(1, &msaaRenderBuffer); }
	if( stencilBuffer ) { glDeleteRenderbuffers(1, &stencilBuffer); }
#else
	if( viewFrameBuffer ) { glDeleteFramebuffers( 1, &viewFrameBuffer); }
	if( viewRenderBuffer ) { glDeleteRenderbuffers(1, &viewRenderBuffer); }
	if( msaaFrameBuffer ) {	glDeleteFramebuffers( 1, &msaaFrameBuffer); }
	if( msaaRenderBuffer ) { glDeleteRenderbuffers(1, &msaaRenderBuffer); }
	if( stencilBuffer ) { glDeleteRenderbuffers(1, &stencilBuffer); }
#endif
	
	path->release();

	sharedGLContext = NULL;
	vertexBuffer = NULL;
}

void EJCanvasContext::create()
{
#ifdef _WINDOWS
	if( msaaEnabled ) {
		glGenFramebuffersEXT(1, &msaaFrameBuffer);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, msaaFrameBuffer);

		glGenRenderbuffersEXT(1, &msaaRenderBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, msaaRenderBuffer);

		//glRenderbufferStorageMultisampleIMG(GL_RENDERBUFFER, msaaSamples, GL_RGBA8, bufferWidth, bufferHeight);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER_EXT, msaaRenderBuffer);
	}

	glGenFramebuffersEXT(1, &viewFrameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, viewFrameBuffer);

	glGenRenderbuffersEXT(1, &viewRenderBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, viewRenderBuffer);

#else
	if( msaaEnabled ) {
		glGenFramebuffers(1, &msaaFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);

		glGenRenderbuffers(1, &msaaRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, msaaRenderBuffer);

		//glRenderbufferStorageMultisampleIMG(GL_RENDERBUFFER_OES, msaaSamples, GL_RGBA8_OES, bufferWidth, bufferHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaRenderBuffer);
	}

	 glGenFramebuffers(1, &viewFrameBuffer);
	 glBindFramebuffer(GL_FRAMEBUFFER, viewFrameBuffer);

	 glGenRenderbuffers(1, &viewRenderBuffer);
	 glBindRenderbuffer(GL_RENDERBUFFER, viewRenderBuffer);
#endif

}

void EJCanvasContext::setScreenSize(int widthp, int heightp)
{
	bufferWidth = viewportWidth = width = widthp;
	bufferHeight = viewportHeight = height = heightp;
}

void EJCanvasContext::createStencilBufferOnce()
{
	if( stencilBuffer ) { return; }
#ifdef _WINDOWS

	glGenRenderbuffersEXT(1, &stencilBuffer);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, stencilBuffer);
	if( msaaEnabled ) {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER_EXT, msaaSamples, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
	}
	else {
		glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
	}
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER_EXT, stencilBuffer);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER_EXT, stencilBuffer);

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, msaaEnabled ? msaaRenderBuffer : viewRenderBuffer );

#else

	glGenRenderbuffers(1, &stencilBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, stencilBuffer);
	if( msaaEnabled ) {
		//TODO: Find Android specific solution
		//glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, msaaSamples, GL_DEPTH24_STENCIL8_OES, bufferWidth, bufferHeight);
	}
	else {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, bufferWidth, bufferHeight);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, stencilBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, msaaEnabled ? msaaRenderBuffer : viewRenderBuffer );

#endif

	glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void EJCanvasContext::bindVertexBuffer()
{
	glEnableVertexAttribArray(kEJGLProgram2DAttributePos);
	glVertexAttribPointer(kEJGLProgram2DAttributePos, 2, GL_FLOAT, GL_FALSE, sizeof(EJVertex), (char *)vertexBuffer + offsetof(EJVertex, pos));
	
	glEnableVertexAttribArray(kEJGLProgram2DAttributeUV);
	glVertexAttribPointer(kEJGLProgram2DAttributeUV, 2, GL_FLOAT, GL_FALSE, sizeof(EJVertex), (char *)vertexBuffer + offsetof(EJVertex, uv));

	glEnableVertexAttribArray(kEJGLProgram2DAttributeColor);
	glVertexAttribPointer(kEJGLProgram2DAttributeColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(EJVertex), (char *)vertexBuffer + offsetof(EJVertex, color));
}

void EJCanvasContext::prepare()
{
	//Bind the frameBuffer and vertexBuffer array

	GLenum error = glGetError();
#ifdef _WINDOWS
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, msaaEnabled ? msaaFrameBuffer : viewFrameBuffer );
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, msaaEnabled ? msaaRenderBuffer : viewRenderBuffer );
#else
	NSLOG("Enter prepare, error: %d", error);
	glBindFramebuffer(GL_FRAMEBUFFER, msaaEnabled ? msaaFrameBuffer : viewFrameBuffer );
	error = glGetError();
	NSLOG("glBindFramebuffer passed, error: %d", error);
	glBindRenderbuffer(GL_RENDERBUFFER, msaaEnabled ? msaaRenderBuffer : viewRenderBuffer );
	error = glGetError();
	NSLOG("glBindRenderbuffer passed, error: %d", error);
#endif	
	NSLOG("Before glViewport call, width: %d, height: %d", width, height);
	glViewport(0, 0, viewportWidth, viewportHeight);
	error = glGetError();
	NSLOG("glViewport passed, error: %d", error);
	
	
	EJCompositeOperation op = state->globalCompositeOperation;
	glBlendFunc( EJCompositeOperationFuncs[op].source, EJCompositeOperationFuncs[op].destination );
	error = glGetError();
	NSLOG("glBlendFunc passed, error: %d", error);
	//glDisable(GL_TEXTURE_2D);
	//error = glGetError();
	//NSLOG("glDisable passed, error: %d", error);
	currentTexture = NULL;
	currentProgram = NULL;
	//TODO: Should be removed? implemented differently?
	EJTexture::setSmoothScaling(imageSmoothingEnabled);
	error = glGetError();
	NSLOG("setSmoothScaling passed, error: %d", error);
	
	bindVertexBuffer();
	error = glGetError();
	NSLOG("bindVertexBuffer passed, error: %d", error);
		
	if( state->clipPath ) {
		glDepthFunc(GL_EQUAL);
		error = glGetError();
		NSLOG("glDepthFunc passed, error: %d", error);
	}
	else {
		glDepthFunc(GL_ALWAYS);
		error = glGetError();
		NSLOG("glDepthFunc passed, error: %d", error);
	}

	//TODO: Should be implemented?	
	//needsPresenting = YES;
}

//TODO: Should be implemented?
//setWidth, setHeight methods

void EJCanvasContext::setTexture(EJTexture * newTexture) {
	if( currentTexture == newTexture ) { return; }
	
	flushBuffers();
	
	//TODO: Keep? not appearing in Ejecta iOS
	if( !newTexture && currentTexture ) {
		// Was enabled; should be disabled
		glDisable(GL_TEXTURE_2D);
	}
	else if( newTexture && !currentTexture ) {
		// Was disabled; should be enabled
		glEnable(GL_TEXTURE_2D);
	}
	
	currentTexture = newTexture;
	if(currentTexture)currentTexture->bind();
}

void EJCanvasContext::setProgram(EJGLProgram2D *newProgram) {
	NSLOG("Entering setProgram");
    if( currentProgram == newProgram ) { return; }
	NSLOG("setProgram - New program");
    
    flushBuffers();
    currentProgram = newProgram;
    
	NSLOG("setProgram - Before getting program");
    glUseProgram(currentProgram->getProgram());
    glUniform2f(currentProgram->getScreen(), width, height * (upsideDown ? -1 : 1));
}

void EJCanvasContext::pushTri(float x1, float y1, float x2, float y2, float x3, float y3, EJColorRGBA color, CGAffineTransform transform)
{
	if( vertexBufferIndex >= vertexBufferSize - 3 ) {
		flushBuffers();
	}
	
	EJVector2 d1 = { x1, y1 };
	EJVector2 d2 = { x2, y2 };
	EJVector2 d3 = { x3, y3 };
	
	if( !CGAffineTransformIsIdentity(transform) ) {
		d1 = EJVector2ApplyTransform( d1, transform );
		d2 = EJVector2ApplyTransform( d2, transform );
		d3 = EJVector2ApplyTransform( d3, transform );
	}
	
	EJVertex * vb = &vertexBuffer[vertexBufferIndex];

	EJVertex vb_0 = {d1, {0.5, 1}, color};
	EJVertex vb_1 = { d2, {0.5, 0.5}, color };
	EJVertex vb_2 = { d3, {0.5, 1}, color };

	vb[0] = vb_0;
	vb[1] = vb_1;
	vb[2] = vb_2;
	
	vertexBufferIndex += 3;
}

void EJCanvasContext::pushQuad(EJVector2 v1, EJVector2 v2, EJVector2 v3, EJVector2 v4, EJVector2 t1, EJVector2 t2, EJVector2 t3, EJVector2 t4, EJColorRGBA color, CGAffineTransform transform)
{
	if( vertexBufferIndex >= vertexBufferSize - 6 ) {
		flushBuffers();
	}
	
	if( !CGAffineTransformIsIdentity(transform) ) {
		v1 = EJVector2ApplyTransform( v1, transform );
		v2 = EJVector2ApplyTransform( v2, transform );
		v3 = EJVector2ApplyTransform( v3, transform );
		v4 = EJVector2ApplyTransform( v4, transform );
	}
	
	EJVertex * vb = &vertexBuffer[vertexBufferIndex];

	EJVertex vb_0 = { v1, t1, color };
	EJVertex vb_1 = { v2, t2, color };
	EJVertex vb_2 = { v3, t3, color };
	EJVertex vb_3 = { v2, t2, color };
	EJVertex vb_4 = { v3, t3, color };
	EJVertex vb_5 = { v4, t4, color };

	vb[0] = vb_0;
	vb[1] = vb_1;
	vb[2] = vb_2;
	vb[3] = vb_3;
	vb[4] = vb_4;
	vb[5] = vb_5;
	
	vertexBufferIndex += 6;
}

void EJCanvasContext::pushRect(float x, float y, float w, float h, float tx, float ty, float tw, float th, EJColorRGBA color, CGAffineTransform transform)
{

	if( vertexBufferIndex >= vertexBufferSize - 6 ) {
		flushBuffers();
	}
	
	EJVector2 d11 = { x, y };
	EJVector2 d21 = { x+w, y };
	EJVector2 d12 = { x, y+h };
	EJVector2 d22 = { x+w, y+h };
	
	if( !CGAffineTransformIsIdentity(transform) ) {
		d11 = EJVector2ApplyTransform( d11, transform );
		d21 = EJVector2ApplyTransform( d21, transform );
		d12 = EJVector2ApplyTransform( d12, transform );
		d22 = EJVector2ApplyTransform( d22, transform );
	}
	
	EJVertex * vb = &vertexBuffer[vertexBufferIndex];

	EJVertex vb_0 = { d11, {0, 0}, color };	// top left
	EJVertex vb_1 = { d21, {0, 0}, color };	// top right
	EJVertex vb_2 = { d12, {0, 0}, color };	// bottom left

	EJVertex vb_3 = { d21, {0, 0}, color };	// top right
	EJVertex vb_4 = { d12, {0, 0}, color };	// bottom left
	EJVertex vb_5 = { d22, {0, 0}, color };// bottom right

	vb[0] = vb_0;	// top left
	vb[1] = vb_1;	// top right
	vb[2] = vb_2;	// bottom left
		
	vb[3] = vb_3;	// top right
	vb[4] = vb_4;	// bottom left
	vb[5] = vb_5;// bottom right
	
	vertexBufferIndex += 6;
}

//TODO: Should be implemented?
//pushFilledRect, pushGradientRect, pushPatternedRect methods

void EJCanvasContext::pushTexturedRect(float x, float y, float w, float h, float tx, float ty, float tw, float th, EJColorRGBA color, CGAffineTransform transform)
{

	if( vertexBufferIndex >= vertexBufferSize - 6 ) {
		flushBuffers();
	}
	
	EJVector2 d11 = { x, y };
	EJVector2 d21 = { x+w, y };
	EJVector2 d12 = { x, y+h };
	EJVector2 d22 = { x+w, y+h };
	
	if( !CGAffineTransformIsIdentity(transform) ) {
		d11 = EJVector2ApplyTransform( d11, transform );
		d21 = EJVector2ApplyTransform( d21, transform );
		d12 = EJVector2ApplyTransform( d12, transform );
		d22 = EJVector2ApplyTransform( d22, transform );
	}
	
	EJVertex * vb = &vertexBuffer[vertexBufferIndex];

	EJVertex vb_0 = { d11, {tx, ty}, color };	// top left
	EJVertex vb_1 = { d21, {tx+tw, ty}, color };	// top right
	EJVertex vb_2 = { d12, {tx, ty+th}, color };	// bottom left

	EJVertex vb_3 = { d21, {tx+tw, ty}, color };	// top right
	EJVertex vb_4 = { d12, {tx, ty+th}, color };	// bottom left
	EJVertex vb_5 = { d22, {tx+tw, ty+th}, color };// bottom right

	vb[0] = vb_0;	// top left
	vb[1] = vb_1;	// top right
	vb[2] = vb_2;	// bottom left
		
	vb[3] = vb_3;	// top right
	vb[4] = vb_4;	// bottom left
	vb[5] = vb_5;// bottom right
	
	vertexBufferIndex += 6;
}

void EJCanvasContext::flushBuffers()
{
	NSLOG("Entering flushBuffers - Flushing: %d", vertexBufferIndex);
	if( vertexBufferIndex == 0 ) { return; }

	NSLOG("flushBuffers - Drawing");
	glDrawArrays(GL_TRIANGLES, 0, vertexBufferIndex);
	//TODO: Should be implemented?
	//needsPresenting = YES;
	vertexBufferIndex = 0;
}

//TODO: Should be implemented?
//imageSmoothingEnabled, setImageSmoothingEnabled, setGlobalCompositeOperation, globalCompositeOperation, setFont, font, setFillObject, fillObject methods

void EJCanvasContext::save()
{
	if( stateIndex == EJ_CANVAS_STATE_STACK_SIZE-1 ) {
		NSLOG("Warning: EJ_CANVAS_STATE_STACK_SIZE (%d) reached", EJ_CANVAS_STATE_STACK_SIZE);
		return;
	}
	
	stateStack[stateIndex+1] = stateStack[stateIndex];
	stateIndex++;
	state = &stateStack[stateIndex];
	state->font->retain();
	//TODO: Should be implemented?
	//[state->fillObject retain];
	if(state->clipPath)state->clipPath->retain();
}

void EJCanvasContext::restore()
{
	if( stateIndex == 0 ) {
		NSLOG("Warning: Can't pop stack at index 0");
		return;
	}
	
	EJCompositeOperation oldCompositeOp = state->globalCompositeOperation;
	EJPath * oldClipPath = state->clipPath;
	
	// Clean up current state
	state->font->release();
	//TODO: Should be implemented?
	//[state->fillObject release];

	if( state->clipPath && state->clipPath != stateStack[stateIndex-1].clipPath ) {
		resetClip();
	}
	if( state->clipPath )state->clipPath->release();
	
	// Load state from stack
	stateIndex--;
	state = &stateStack[stateIndex];
	
    path->transform = state->transform;
    
	// Set Composite op, if different
	if( state->globalCompositeOperation != oldCompositeOp ) {
		globalCompositeOperation = state->globalCompositeOperation;
	}
	
	// Render clip path, if present and different
	if( state->clipPath && state->clipPath != oldClipPath ) {
		setProgram(sharedGLContext->getGlProgram2DFlat());
		state->clipPath->drawPolygonsToContext(this,kEJPathPolygonTargetDepth);
	}
}

void EJCanvasContext::rotate(float angle)
{
	state->transform = CGAffineTransformRotate( state->transform, angle );
    path->transform = state->transform;
}

void EJCanvasContext::translate(float x, float y)
{
	state->transform = CGAffineTransformTranslate( state->transform, x, y );
    path->transform = state->transform;
}

void EJCanvasContext::scale(float x, float y)
{
	state->transform = CGAffineTransformScale( state->transform, x, y );
	path->transform = state->transform;
}

void EJCanvasContext::transform(float m11, float m12, float m21, float m22, float dx, float dy)
{
	CGAffineTransform t = CGAffineTransformMake( m11, m12, m21, m22, dx, dy );
	state->transform = CGAffineTransformConcat( t, state->transform );
	path->transform = state->transform;
}

void EJCanvasContext::setTransform(float m11, float m12, float m21, float m22, float dx, float dy)
{
	state->transform = CGAffineTransformMake( m11, m12, m21, m22, dx, dy );
	path->transform = state->transform;
}

void EJCanvasContext::drawImage(EJTexture * texture, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh)
{
	if (texture)
	{
		float tw = texture->realWidth;
		float th = texture->realHeight;

		setProgram(sharedGLContext->getGlProgram2DTexture());

		//EJColorRGBA color = {{255, 255, 255, 255 * state->globalAlpha}};
		EJColorRGBA color = {0xffffffff};
		color.rgba.a = (unsigned char)(255 * state->globalAlpha);
		setTexture(texture);
		//TODO: Should be implemented: EJCanvasBlendWhiteColor(state)
		//pushTexturedRect(dx, dy, dw, dh, sx/tw, sy/th, sw/tw, sh/th, EJCanvasBlendWhiteColor(state), state->transform);
		pushTexturedRect(dx, dy, dw, dh, sx/tw, sy/th, sw/tw, sh/th, color, state->transform);
	}
}

void EJCanvasContext::fillRect(float x, float y, float w, float h)
{
	NSLOG("Entering fillRect: %d, %d, %d, %d", x, y, w, h);
	//TODO: Should be implemented?
	// if( state->fillObject ) {
	// 	[self pushFilledRectX:x y:y w:w h:h fillable:state->fillObject
	// 		color:EJCanvasBlendWhiteColor(state) withTransform:state->transform];
	// }
	// else {
		//TODO: Should be removed? or placed after setProgram?
		setTexture(NULL);
		
		setProgram(sharedGLContext->getGlProgram2DFlat());

		EJColorRGBA color = state->fillColor;	
		color.rgba.a = (unsigned char)(color.rgba.a * state->globalAlpha);
		//TODO: Should be implemented: EJCanvasBlendWhiteColor(state)
		//pushRect(x, y, w, h, 0, 0, 0, 0, EJCanvasBlendFillColor(state), state->transform);
		pushRect(x, y, w, h, 0, 0, 0, 0, color, state->transform);
	// }
}

void EJCanvasContext::strokeRect(float x, float y, float w, float h)
{
	// strokeRect should not affect the current path, so we create
	// a new, tempPath instead.
	EJPath * tempPath = new EJPath();
	tempPath->transform = state->transform;
	
	tempPath->moveTo(x, y);
	tempPath->lineTo(x+w, y);
	tempPath->lineTo(x+w, y+h);
	tempPath->lineTo(x, y+h);
	tempPath->close();
	
	setProgram(sharedGLContext->getGlProgram2DFlat());
	tempPath->drawLinesToContext(this);
	tempPath->release();

}

void EJCanvasContext::clearRect(float x, float y, float w, float h)
{
	//TODO: Should be removed? or placed after setProgram?
	setTexture(NULL);
	
	setProgram(sharedGLContext->getGlProgram2DFlat());

	EJCompositeOperation oldOp = state->globalCompositeOperation;
	globalCompositeOperation = kEJCompositeOperationDestinationOut;
	
	static EJColorRGBA white = {0x00000000};
	pushRect(x, y, w, h, 0, 0, 0, 0, white, state->transform);
	
	globalCompositeOperation = oldOp;

}

//TODO: Reimplement it as Ejecta iOS?
//getImageDataScaled, getImageDataSx, getImageDataHDSx
EJImageData* EJCanvasContext::getImageData(float sx, float sy, float sw, float sh)
{
	flushBuffers();
	GLubyte * pixels = (GLubyte*)malloc( (size_t)sw * (size_t)sh * 4 * sizeof(GLubyte));
	glReadPixels((GLint)sx, (GLint)sy, (GLsizei)sw, (GLsizei)sh, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	EJImageData* imageData = new EJImageData((int)sw, (int)sh, pixels);
	imageData->autorelease();
	return imageData;
}

void EJCanvasContext::putImageData(EJImageData* imageData, float dx, float dy)
{
	EJTexture * texture = imageData->texture();
	setProgram(sharedGLContext->getGlProgram2DTexture());
	setTexture(texture);
	
	short tw = texture->realWidth;
	short th = texture->realHeight;
	
	static EJColorRGBA white = {0xffffffff};
	
	//TODO: reimplement it with pushTexturedRect
	pushTexturedRect(dx, dy, tw, th, 0, 0, 1, 1, white, CGAffineTransformIdentity);
	flushBuffers();
}

//TODO: Should be implemented?
//multiple putImageData methods

void EJCanvasContext::beginPath()
{
	path->reset();
}

void EJCanvasContext::closePath()
{
	path->close();
}

void EJCanvasContext::fill()
{
	setProgram(sharedGLContext->getGlProgram2DFlat());
	path->drawPolygonsToContext(this,  kEJPathPolygonTargetColor);
}

void EJCanvasContext::stroke()
{
	setProgram(sharedGLContext->getGlProgram2DFlat());
	path->drawLinesToContext(this);
}

void EJCanvasContext::moveTo(float x, float y)
{
	path->moveTo(x, y);
}

void EJCanvasContext::lineTo(float x, float y)
{
	path->lineTo(x, y);
}

void EJCanvasContext::bezierCurveTo(float cpx, float cpy, float cpx2, float cpy2, float x, float y)
{
	float scale = CGAffineTransformGetScale( state->transform );
	//TODO: Should be implemented
	//path->bezierCurveTo(cpx, cpy, x, y, scale);
	path->quadraticCurveTo(cpx, cpy, x, y, scale);
}

void EJCanvasContext::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
	float scale = CGAffineTransformGetScale( state->transform );
	path->quadraticCurveTo(cpx, cpy, x, y, scale);
}

void EJCanvasContext::rect(float x, float y, float w, float h)
{
	path->moveTo(x, y);
	path->lineTo(x+w, y);
	path->lineTo(x+w, y+h);
	path->lineTo(x, y+h);
	path->close();
}

void EJCanvasContext::arcTo(float x1, float y1, float x2, float y2, float radius)
{
	path->arcTo(x1, y1, x2, y2, radius);
}

void EJCanvasContext::arc(float x, float y, float radius, float startAngle, float endAngle, BOOL antiClockwise)
{
	path->arc(x, y, radius, startAngle, endAngle, antiClockwise);
}

//TODO: Rename/reimplement to match Ejecta iOS
EJFont* EJCanvasContext::acquireFont(NSString* fontName , float pointSize ,BOOL fill ,float contentScale) {	
	//NSString * cacheKey = NSString::createWithFormat("%s_%.2f_%d_%.2f", fontName->getCString(), pointSize, fill, contentScale);
	EJFont * font = (EJFont *)fontCache->objectForKey(fontName->getCString());	
	if( !font ) {
		font =new EJFont(fontName,pointSize ,fill ,contentScale);		
		fontCache->setObject(font,fontName->getCString());
		font->release();
	}else{
		font->setFill(fill);
	}
	return font;
}

void EJCanvasContext::fillText(NSString * text, float x, float y)
{
	EJFont *font =acquireFont(state->font->fontName ,state->font->pointSize,true,backingStoreRatio);	

	setProgram(sharedGLContext->getGlProgram2DAlphaTexture());
	font->drawString(text, this, x, y);
}

//TODO: Reimplement to match Ejecta iOS
void EJCanvasContext::strokeText(NSString * text, float x, float y)
{
	EJFont *font =acquireFont(state->font->fontName ,state->font->pointSize,false,backingStoreRatio);

	setProgram(sharedGLContext->getGlProgram2DAlphaTexture());
	font->drawString(text, this, x, y);	
	fillText(text,x,y);
}

float EJCanvasContext::measureText(NSString * text)
{
	EJFont *font =acquireFont(state->font->fontName ,state->font->pointSize,true,backingStoreRatio);
	return font->measureString(text);
}

void EJCanvasContext::clip()
{
	flushBuffers();
	state->clipPath->release();
	state->clipPath = NULL;
	
	state->clipPath = (EJPath*)(path->copy());
	setProgram(sharedGLContext->getGlProgram2DFlat());
	state->clipPath->drawPolygonsToContext(this, kEJPathPolygonTargetDepth);
}

void EJCanvasContext::resetClip()
{
	if( state->clipPath ) {
		flushBuffers();
		state->clipPath->release();
		state->clipPath = NULL;
		
		glDepthMask(GL_TRUE);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_ALWAYS);
	}
}