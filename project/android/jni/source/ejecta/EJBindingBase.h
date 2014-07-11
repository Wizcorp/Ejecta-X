/****************************************************************************

 ****************************************************************************/

#ifndef __EJ_BINDING_BASE_H__
#define __EJ_BINDING_BASE_H__

#include "EJCocoa/NSObject.h"
#include "EJApp.h"
#include "EJConvert.h"
#include "EJConvert.h"

extern JSValueRef ej_global_undefined;

// All classes derived from this JS_BaseClass will return a JSClassRef through the 
// 'getJSClass' class method. The properties and functions that are exposed to 
// JavaScript are defined through the 'staticFunctions' and 'staticValues' in this 
// JSClassRef.

// Since these functions don't have extra data (e.g. a void*), we have to define a 
// C callback function for each js function, for each js getter and for each js setter.

// Furthermore, a class method is added to the objc class to return the function pointer
// to the particular C callback function - this way we can later inflect the objc class
// and gather all function pointers.

// The class method that returns a pointer to the static C callback function
#define __EJ_GET_POINTER_TO(NAME) \
	SEL _ptr_to##NAME() { \
		return (SEL)NAME; \
	} \
	REFECTION_FUNCTION_IMPLEMENT(_ptr_to##NAME);

// ------------------------------------------------------------------------------------
// Function - use with EJ_BIND_FUNCTION( functionName, ctx, argc, argv ) { ... }
#define EJ_BIND_FUNCTION_DEFINE(NAME, CTX_NAME, ARGC_NAME, ARGV_NAME) \
	JSValueRef _func_##NAME(JSContextRef CTX_NAME, size_t ARGC_NAME, const JSValueRef* ARGV_NAME)

#define EJ_BIND_FUNCTION(CLASS, NAME, CTX_NAME, ARGC_NAME, ARGV_NAME) \
	\
	static JSValueRef _##CLASS##_func_##NAME( \
		JSContextRef ctx, \
		JSObjectRef function, \
		JSObjectRef object, \
		size_t argc, \
		const JSValueRef argv[], \
		JSValueRef* exception \
	) { \
		CLASS* instance = (CLASS*)(JSObjectGetPrivate(object)); \
		JSValueRef ret = (JSValueRef)instance->_func_##NAME(ctx, argc, argv); \
		return ret ? ret : ej_global_undefined; \
	} \
	__EJ_GET_POINTER_TO(_##CLASS##_func_##NAME)\
	\
	/* The actual implementation for this method */ \
	JSValueRef CLASS::_func_##NAME(JSContextRef CTX_NAME, size_t ARGC_NAME, const JSValueRef* ARGV_NAME)

// ------------------------------------------------------------------------------------
// Getter - use with EJ_BIND_GET( propertyName, ctx ) { ... }
#define EJ_BIND_GET_DEFINE(NAME, CTX_NAME) \
	JSValueRef _get_##NAME( JSContextRef CTX_NAME)

#define EJ_BIND_GET(CLASS, NAME, CTX_NAME) \
 	\
 	/* The C callback function for the exposed getter and class method that returns it */ \
 	static JSValueRef _##CLASS##_get_##NAME( \
 		JSContextRef ctx, \
 		JSObjectRef object, \
 		JSStringRef propertyName, \
 		JSValueRef* exception \
 	) { \
		CLASS* instance = (CLASS*)(JSObjectGetPrivate(object)); \
		JSValueRef ret = (JSValueRef)instance->_get_##NAME(ctx); \
		return ret ? ret : ej_global_undefined; \
 	} \
 	__EJ_GET_POINTER_TO(_##CLASS##_get_##NAME)\
 	\
 	/* The actual implementation for this getter */ \
 	JSValueRef CLASS::_get_##NAME(JSContextRef CTX_NAME)

// ------------------------------------------------------------------------------------
// Setter - use with EJ_BIND_SET( propertyName, ctx, value ) { ... }
#define EJ_BIND_SET_DEFINE(NAME, CTX_NAME, VALUE_NAME) \
		void _set_##NAME( JSContextRef CTX_NAME,JSValueRef VALUE_NAME)

#define EJ_BIND_SET(CLASS, NAME, CTX_NAME, VALUE_NAME) \
 	\
 	/* The C callback function for the exposed setter and class method that returns it */ \
 	static bool _##CLASS##_set_##NAME( \
 		JSContextRef ctx, \
 		JSObjectRef object, \
 		JSStringRef propertyName, \
 		JSValueRef value, \
 		JSValueRef* exception \
 	) { \
 		CLASS* instance = (CLASS*)(JSObjectGetPrivate(object)); \
 		instance->_set_##NAME(ctx,value); \
 		return true; \
 	} \
 	__EJ_GET_POINTER_TO(_##CLASS##_set_##NAME) \
 	\
 	/* The actual implementation for this setter */ \
 	void CLASS::_set_##NAME(JSContextRef CTX_NAME,JSValueRef VALUE_NAME)

// // ------------------------------------------------------------------------------------
// // Shorthand to define a function that logs a "not implemented" warning
//

#define EJ_BIND_FUNCTION_NOT_IMPLEMENTED_DEFINE(NAME) \
 	EJ_BIND_FUNCTION_DEFINE( NAME, ctx, argc, argv ) 

#define EJ_BIND_FUNCTION_NOT_IMPLEMENTED(CALSS, NAME) \
	EJ_BIND_FUNCTION( CALSS, NAME, ctx, argc, argv ) { \
		static bool didShowWarning; \
		if( !didShowWarning ) { \
			NSLOG("Warning: method %s() is not yet implemented!", #NAME); \
			didShowWarning = true; \
		} \
		return NULL; \
	}

// ------------------------------------------------------------------------------------
// Shorthand to bind enums with name tables
#define EJ_BIND_ENUM_DEFINE( NAME, ENUM_NAMES, TARGET ) \
 	EJ_BIND_GET_DEFINE( NAME, ctx ); \
 	EJ_BIND_SET_DEFINE( NAME, ctx, value ) 

#define EJ_BIND_ENUM( CLASS,NAME, ENUM_NAMES, TARGET ) \
 	EJ_BIND_GET( CLASS,NAME, ctx ) { \
 		JSStringRef src = JSStringCreateWithUTF8CString( ENUM_NAMES##Names[TARGET] ); \
 		JSValueRef ret = JSValueMakeString(ctx, src); \
 		JSStringRelease(src); \
 		return ret; \
 	} \
 	\
 	EJ_BIND_SET( CLASS,NAME, ctx, value ) { \
 		JSStringRef str = JSValueToStringCopy(ctx, value, NULL); \
 		const JSChar * strptr = JSStringGetCharactersPtr( str ); \
 		int length = JSStringGetLength(str)-1; \
 		for( int i = 0; i < sizeof(ENUM_NAMES##Names)/sizeof(ENUM_NAMES##Names[0]); i++ ) { \
 			if( JSStrIsEqualToStr( strptr, ENUM_NAMES##Names[i], length) ) { \
 				TARGET = (ENUM_NAMES)i; \
 				break; \
 			} \
 		} \
 		JSStringRelease( str );\
 	}

//Binding needs to be modified to make use of accessors appropriately
#define EJ_BIND_ENUM_GETTER( CLASS,NAME, ENUM_NAMES, TARGET ) \
 	EJ_BIND_GET( CLASS,NAME, ctx ) { \
 		JSStringRef src = JSStringCreateWithUTF8CString( ENUM_NAMES##Names[TARGET()] ); \
 		JSValueRef ret = JSValueMakeString(ctx, src); \
 		JSStringRelease(src); \
 		return ret; \
 	} \

#define EJ_BIND_ENUM_SETTER( CLASS,NAME, ENUM_NAMES, TARGET ) \
 	EJ_BIND_SET( CLASS,NAME, ctx, value ) { \
 		JSStringRef str = JSValueToStringCopy(ctx, value, NULL); \
 		const JSChar * strptr = JSStringGetCharactersPtr( str ); \
 		int length = JSStringGetLength(str)-1; \
 		for( int i = 0; i < sizeof(ENUM_NAMES##Names)/sizeof(ENUM_NAMES##Names[0]); i++ ) { \
 			if( JSStrIsEqualToStr( strptr, ENUM_NAMES##Names[i], length) ) { \
 				TARGET((ENUM_NAMES)i); \
 				break; \
 			} \
 		} \
 		JSStringRelease( str );\
 	}

static inline bool JSStrIsEqualToStr(const JSChar * s1, const char * s2,
		int length) {
	for (int i = 0; i < length && *s1 == *s2; i++) {
		s1++;
		s2++;
	}
	return (*s1 == *s2);
}

// ------------------------------------------------------------------------------------
// Shorthand to bind const numbers

#define EJ_BIND_CONST(NAME, VALUE) \
 	static JSValueRef _get_##NAME( \
 		JSContextRef ctx, \
 		JSObjectRef object, \
 		JSStringRef propertyName, \
 		JSValueRef* exception \
 	) { \
 		return JSValueMakeNumber(ctx, VALUE); \
 	} \
 	__EJ_GET_POINTER_TO(_get_##NAME)

class EJBindingBase: public NSObject {
protected:
	JSObjectRef jsObject;
public:
	EJBindingBase();
	EJBindingBase(JSContextRef ctxp, JSObjectRef obj, size_t argc, const JSValueRef argv[]);
	~EJBindingBase();

	virtual string superclass(){return 0;};

	virtual void initWithContext(JSContextRef ctxp, JSObjectRef obj, size_t argc, const JSValueRef argv[]);
	static JSObjectRef createJSObjectWithContext(JSContextRef ctx, EJBindingBase* instance);
	static JSClassRef getJSClass(EJBindingBase* ej_obj);
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingBase);
};

#endif // __EJ_BINDING_BASE_H_
