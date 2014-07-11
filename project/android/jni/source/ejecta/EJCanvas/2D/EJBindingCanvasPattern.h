#ifndef __EJ_BINDING_CANVASPATTERN_H__
#define __EJ_BINDING_CANVASPATTERN_H__
#include "EJBindingBase.h"
#include "EJCanvasPattern.h"

class EJBindingCanvasPattern : public EJBindingBase 
{
private:
	EJCanvasPattern *pattern;
public:
	EJBindingCanvasPattern();
	EJBindingCanvasPattern(JSContextRef ctx, JSObjectRef obj, size_t argc, const JSValueRef argv[]);
	~EJBindingCanvasPattern();
	virtual string superclass(){return EJBindingBase::toString();};
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingCanvasPattern);
	static JSObjectRef createJSObjectWithContext(JSContextRef ctx, EJCanvasPattern* pPattern);
	static EJCanvasPattern* patternFromJSValue(JSValueRef value);
};

/*
+ (JSObjectRef)createJSObjectWithContext:(JSContextRef)ctx
	scriptView:(EJJavaScriptView *)scriptView
	pattern:(EJCanvasPattern *)pattern;
+ (EJCanvasPattern *)patternFromJSValue:(JSValueRef)value;

@end


	EJImageData* m_imageData;
public:

	EJBindingImageData(JSContextRef ctx, JSObjectRef obj, EJImageData* imageData);
	EJBindingImageData();
	~EJBindingImageData();
	
	virtual string superclass(){return EJBindingBase::toString();};

	EJImageData* imageData();
	virtual void init(JSContextRef ctx, JSObjectRef obj, EJImageData* imageData);

	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingImageData);

	EJ_BIND_GET_DEFINE( data, ctx );
	EJ_BIND_GET_DEFINE( width, ctx );
	EJ_BIND_GET_DEFINE( height, ctx );
*/

#endif