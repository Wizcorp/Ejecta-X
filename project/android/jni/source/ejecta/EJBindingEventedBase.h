#ifndef __EJ_BINDING_EVENTED_BASE_H__
#define __EJ_BINDING_EVENTED_BASE_H__

#include "EJBindingBase.h"

// ------------------------------------------------------------------------------------
// Events; shorthand for EJ_BIND_GET/SET - use with EJ_BIND_EVENT( eventname );

#define EJ_BIND_EVENT(CLASS,NAME) \
	static JSValueRef _##CLASS##_get_on##NAME( \
		JSContextRef ctx, \
		JSObjectRef object, \
		JSStringRef propertyName, \
		JSValueRef* exception \
	) { \
		CLASS* instance = (CLASS*)(JSObjectGetPrivate(object)); \
		return (JSValueRef)instance->getCallbackWith(NSStringMake(#NAME),ctx); \
	} \
	__EJ_GET_POINTER_TO(_##CLASS##_get_on##NAME) \
	\
	static bool _##CLASS##_set_on##NAME( \
		JSContextRef ctx, \
		JSObjectRef object, \
		JSStringRef propertyName, \
		JSValueRef value, \
		JSValueRef* exception \
	) { \
		CLASS* instance = (CLASS*)(JSObjectGetPrivate(object)); \
		instance->setCallbackWith(NSStringMake(#NAME),ctx, value); \
		return true; \
	} \
	__EJ_GET_POINTER_TO(_##CLASS##_set_on##NAME)

typedef struct {
	const char *name;
	JSValueRef value;
} JSEventProperty;

class EJBindingEventedBase : public EJBindingBase {
	NSDictionary * eventListeners; // for addEventListener
	NSDictionary * onCallbacks; // for on* setters
public:
	EJBindingEventedBase();
	EJBindingEventedBase(JSContextRef ctxp,JSObjectRef obj,size_t argc ,
			const JSValueRef argv[] ) ;
	~EJBindingEventedBase();
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingEventedBase);

	virtual string superclass(){return EJBindingBase::toString();};

	JSObjectRef getCallbackWith(NSString * name, JSContextRef ctx);
	void setCallbackWith(NSString * name, JSContextRef ctx,	JSValueRef callback);
	void triggerEvent(NSString * eventType, int argc, JSValueRef argv[]);
	void triggerEvent(NSString * eventType, int argc, JSEventProperty properties[]);
	void triggerEvent(NSString* eventType);

	EJ_BIND_FUNCTION_DEFINE( addEventListener, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(removeEventListener, ctx, argc, argv);
};

class EJBindingEvent : public EJBindingBase {
	NSString *type;
	JSObjectRef jsTarget;
public:
	EJBindingEvent();
	~EJBindingEvent();
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingEvent);
	virtual string superclass(){return EJBindingBase::toString();};
	static JSObjectRef createJSObjectWithContext(JSContextRef ctx, NSString* type, JSObjectRef target);

	EJ_BIND_GET_DEFINE(target, ctx);
	EJ_BIND_GET_DEFINE(currentTarget, ctx );
	EJ_BIND_GET_DEFINE(type, ctx );

	EJ_BIND_FUNCTION_DEFINE(preventDefault, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(stopPropagation, ctx, argc, argv);
};

#endif // __EJ_BINDING_EVENTED_BASE_H__
