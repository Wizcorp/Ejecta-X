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
	void setCallbackWith(NSString * name, JSContextRef ctx,
			JSValueRef callback);
	void triggerEvent(NSString * name, int argc, JSValueRef argv[]);

	EJ_BIND_FUNCTION_DEFINE( addEventListener, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(removeEventListener, ctx, argc, argv);
};
