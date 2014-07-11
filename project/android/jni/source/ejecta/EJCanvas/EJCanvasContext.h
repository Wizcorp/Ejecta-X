/****************************************************************************

****************************************************************************/

#ifndef __EJ_CANVAS_CONTEXT_H__
#define __EJ_CANVAS_CONTEXT_H__

#ifdef _WINDOWS
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#else
#include <GLES2/gl2.h>
#endif

#include "EJCocoa/support/nsMacros.h"
#include "EJTexture.h"
#include "EJImageData.h"
#include "EJPath.h"
#include "EJCanvas2DTypes.h"
#include "EJFont.h"
#include "EJCocoa/NSDictionary.h"
#include "EJCocoa/NSCache.h"
#include "EJCocoa/UIFont.h"
#include "EJSharedOpenGLContext.h"


#define EJ_CANVAS_STATE_STACK_SIZE 16

class EJPath;

typedef enum {
	kEJLineCapButt,
	kEJLineCapRound,
	kEJLineCapSquare
} EJLineCap;

typedef enum {
	kEJLineJoinMiter,
	kEJLineJoinBevel,
	kEJLineJoinRound
} EJLineJoin;

typedef enum {
	kEJTextBaselineAlphabetic,
	kEJTextBaselineMiddle,
	kEJTextBaselineTop,
	kEJTextBaselineHanging,
	kEJTextBaselineBottom,
	kEJTextBaselineIdeographic
} EJTextBaseline;

typedef enum {
	kEJTextAlignStart,
	kEJTextAlignEnd,
	kEJTextAlignLeft,
	kEJTextAlignCenter,
	kEJTextAlignRight
} EJTextAlign;

typedef enum {
	kEJCompositeOperationSourceOver,
	kEJCompositeOperationLighter,
	kEJCompositeOperationDarker,
	kEJCompositeOperationDestinationOut,
	kEJCompositeOperationDestinationOver,
	kEJCompositeOperationSourceAtop,
	kEJCompositeOperationXOR
} EJCompositeOperation;

static const struct { GLenum source; GLenum destination; float alphaFactor; } EJCompositeOperationFuncs[] = {
	{GL_ONE, GL_ONE_MINUS_SRC_ALPHA, 1},
	{GL_ONE, GL_ONE_MINUS_SRC_ALPHA, 0},
	{GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA, 1},
	{GL_ZERO, GL_ONE_MINUS_SRC_ALPHA, 1},
	{GL_ONE_MINUS_DST_ALPHA, GL_ONE, 1},
	{GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 1},
	{GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA, 1}
};

class EJCanvasPattern;

class EJFillable : public NSObject
{
};

typedef struct {
	CGAffineTransform transform;
	
	EJCompositeOperation globalCompositeOperation;
	EJColorRGBA fillColor;
	EJFillable* fillObject;
	EJColorRGBA strokeColor;
	float globalAlpha;
	
	float lineWidth;
	EJLineCap lineCap;
	EJLineJoin lineJoin;
	float miterLimit;
	
	EJTextAlign textAlign;
	EJTextBaseline textBaseline;
	UIFont* font;
	
	EJPath * clipPath;
} EJCanvasState;

static inline EJColorRGBA EJCanvasBlendColor( EJCanvasState *state, EJColorRGBA color ) {
	float alpha = state->globalAlpha * (float)color.rgba.a/255.0f;
	EJColorRGBA blendedColor;
	blendedColor.rgba.r = (unsigned char)(color.rgba.r * alpha);
	blendedColor.rgba.g = (unsigned char)(color.rgba.g * alpha);
	blendedColor.rgba.b = (unsigned char)(color.rgba.b * alpha);
	blendedColor.rgba.a = (unsigned char)(EJCompositeOperationFuncs[state->globalCompositeOperation].alphaFactor * color.rgba.a * state->globalAlpha);
	return blendedColor;
}

static inline EJColorRGBA EJCanvasBlendWhiteColor( EJCanvasState *state ) {
	return EJCanvasBlendColor(state, (EJColorRGBA){0xffffffff});
}

static inline EJColorRGBA EJCanvasBlendFillColor( EJCanvasState *state ) {
	return EJCanvasBlendColor(state, state->fillColor);
}

static inline EJColorRGBA EJCanvasBlendStrokeColor( EJCanvasState *state ) {
	return EJCanvasBlendColor(state, state->strokeColor);
}

class EJCanvasContext : public NSObject {
protected:
	GLuint viewFrameBuffer, viewRenderBuffer;
	GLuint msaaFrameBuffer, msaaRenderBuffer;
	GLuint stencilBuffer;
	
	short width, height;
	short bufferWidth, bufferHeight;
	
	EJTexture * currentTexture;
	
	EJPath * path;
	
	EJVertex *vertexBuffer;
	int vertexBufferSize;
	int vertexBufferIndex;
	
	int stateIndex;
	EJCanvasState stateStack[EJ_CANVAS_STATE_STACK_SIZE];
	
	bool upsideDown;

	EJGLProgram2D *currentProgram;
	EJSharedOpenGLContext *sharedGLContext;
	EJFillable* fillObject;

	void setProgram(EJGLProgram2D *program);

public:
	NSCache * fontCache;

	EJCanvasState * state;
	UIFont * font;
	float backingStoreRatio;
	bool msaaEnabled;
	int msaaSamples;
	bool imageSmoothingEnabled;

	EJCanvasContext();
	EJCanvasContext(short widthp, short heightp);
	~EJCanvasContext();
	virtual void create();
	virtual void resizeToWidth(short newWidth, short newHeight);
	void setScreenSize(int widthp, int heightp);
	void createStencilBufferOnce();
	void bindVertexBuffer();
	virtual void prepare();
	void setTexture(EJTexture * newTexture);
	void pushTri(float x1, float y1, float x2, float y2, float x3, float y3, EJColorRGBA color, CGAffineTransform transform);
	void pushQuad(EJVector2 v1, EJVector2 v2, EJVector2 v3, EJVector2 v4, EJVector2 t1, EJVector2 t2, EJVector2 t3, EJVector2 t4, EJColorRGBA color, CGAffineTransform transform);
	void pushRect(float x, float y, float w, float h, float tx, float ty, float tw, float th, EJColorRGBA color, CGAffineTransform transform);
	void pushFilledRect(float x, float y, float w, float h, EJFillable* fillable, EJColorRGBA color, CGAffineTransform transform);
	void pushTexturedRect(float x, float y, float w, float h, float tx, float ty, float tw, float th, EJColorRGBA color, CGAffineTransform transform);
	void pushPatternedRect(float x, float y, float w, float h, EJCanvasPattern* pattern, EJColorRGBA color, CGAffineTransform transform);
	void flushBuffers();
	
	void save();
	void restore();
	void rotate(float angle);
	void translate(float x, float y);
	void scale(float x, float y);
	void transform(float m11, float m12, float m21, float m22, float dx, float dy);
	void setTransform(float m11, float m12, float m21, float m22, float dx, float dy);
	void drawImage(EJTexture * texture, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh);
	void fillRect(float x, float y, float w, float h);
	void strokeRect(float x, float y, float w, float h);
	void clearRect(float x, float y, float w, float h);
	virtual EJImageData* getImageData(float sx, float sy, float sw, float sh);
	void putImageData(EJImageData* imageData, float dx, float dy);
	void beginPath();
	void closePath();
	void fill();
	void stroke();
	void moveTo(float x, float y);
	void lineTo(float x, float y);
	void rect(float x, float y, float w, float h);
	void bezierCurveTo(float cpx, float cpy, float cpx2, float cpy2, float x, float y);
	void quadraticCurveTo(float cpx, float cpy, float x, float y);
	void arcTo(float x1, float y1, float x2, float y2, float radius);
	void arc(float x, float y, float radius, float startAngle, float endAngle, BOOL antiClockwise);
	EJFont* acquireFont(NSString* fontName , float pointSize ,BOOL fill ,float contentScale);
	void fillText(NSString * text, float x, float y);
	void strokeText(NSString * text, float x, float y);
	float measureText(NSString * text);

	void clip();
	void resetClip();

	//返回类名
	virtual const char* getClassName();

	void setGlobalCompositeOperation(EJCompositeOperation op);
	EJCompositeOperation getGlobalCompositeOperation() const;
	void setWidth(short w);
	short getWidth() const;
	void setHeight(short h);
	short getHeight() const;
	void setFillObject(EJFillable* pFillable);
	EJFillable* getFillObject();
};

#endif // __EJ_CANVAS_CONTEXT_H__
