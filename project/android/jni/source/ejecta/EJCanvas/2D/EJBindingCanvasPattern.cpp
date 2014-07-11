#include "EJBindingCanvasPattern.h"
#include <typeinfo>

EJBindingCanvasPattern::EJBindingCanvasPattern()
{
	pattern = NULL;
}

EJBindingCanvasPattern::EJBindingCanvasPattern(JSContextRef ctx, JSObjectRef obj, size_t argc, const JSValueRef argv[])
	:EJBindingBase(ctx, obj, argc ,argv)
{
	pattern = NULL;
}

EJBindingCanvasPattern::~EJBindingCanvasPattern()
{
	if(pattern!=NULL)
	{
		pattern->release();
	}
}

JSObjectRef EJBindingCanvasPattern::createJSObjectWithContext(JSContextRef ctx, EJCanvasPattern* pPattern)
{
	if(pPattern == NULL)
	{
		return NULL;
	}

	EJBindingCanvasPattern* binding = new EJBindingCanvasPattern(ctx, NULL, 0, NULL);
	pPattern->retain();
	binding->pattern = pPattern;
 	// Create the JS object
 	JSClassRef bindingClass = EJApp::instance()->getJSClassForClass(binding);
 	JSObjectRef obj = JSObjectMake( ctx, bindingClass, NULL );
 	JSValueProtect(ctx, obj);
 	// Attach the native instance to the js object
 	JSObjectSetPrivate( obj, (void *)binding );
 	JSValueUnprotect(ctx, obj);

	return obj;
}

EJCanvasPattern* EJBindingCanvasPattern::patternFromJSValue(JSValueRef value)
{
	if( !value )
	{
		return NULL;
	}

	EJBindingCanvasPattern *binding = (EJBindingCanvasPattern *)JSObjectGetPrivate((JSObjectRef)value);

	if(binding && typeid(*binding) == typeid(EJBindingCanvasPattern))
	{
		return binding->pattern;
	}

	return NULL;
}

REFECTION_CLASS_IMPLEMENT(EJBindingCanvasPattern);

