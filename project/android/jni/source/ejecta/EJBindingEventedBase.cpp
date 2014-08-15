#include "EJBindingEventedBase.h"


EJBindingEventedBase::EJBindingEventedBase() {

	eventListeners =  NSDictionary::create();
    if (eventListeners != NULL)
    {
        eventListeners->retain();
    }
	onCallbacks =  NSDictionary::create();
    if (onCallbacks != NULL)
    {
        onCallbacks->retain();
    }

}

EJBindingEventedBase::EJBindingEventedBase(JSContextRef ctxp, JSObjectRef obj,
		size_t argc, const JSValueRef argv[]) {

	eventListeners =  NSDictionary::create();
    if (eventListeners != NULL)
    {
        eventListeners->retain();
    }
	onCallbacks =  NSDictionary::create();
    if (onCallbacks != NULL)
    {
        onCallbacks->retain();
    }
}

//
EJBindingEventedBase::~EJBindingEventedBase() {
	JSContextRef ctx = EJApp::instance()->jsGlobalContext;

	// Unprotect all event callbacks
	NSDictElement* eElement = NULL;
	NSDICT_FOREACH(eventListeners, eElement) {
		NSArray * listeners = (NSArray *) eElement->getObject()->copy();

		for (unsigned int var = 0; var < listeners->count(); ++var) {
			NSValue * callbackValue = (NSValue *) listeners->objectAtIndex(var);
			JSValueUnprotect(ctx, (JSValueRef) callbackValue->pointerValue());
		}
	}
	eventListeners->release();

	// Unprotect all event callbacks
	NSDictElement* oElement = NULL;
	NSDICT_FOREACH(onCallbacks, oElement) {
		NSValue * listener = (NSValue *) oElement->getObject()->copy();
		if(listener)
		JSValueUnprotect(ctx, (JSValueRef) listener->pointerValue());
	}
	onCallbacks->release();
}
//
JSObjectRef EJBindingEventedBase::getCallbackWith(NSString * name,
		JSContextRef ctx) {
	NSValue * listener = (NSValue *) onCallbacks->objectForKey(
			name->getCString());
	return listener ? (JSObjectRef) listener->pointerValue() : NULL;
}
//
void EJBindingEventedBase::setCallbackWith(NSString * name, JSContextRef ctx,
		JSValueRef callbackValue) {
	// remove old event listener?
	JSObjectRef oldCallback = getCallbackWith(name, ctx);
	if (oldCallback) {
		JSValueUnprotect(ctx, oldCallback);
		onCallbacks->removeObjectForKey(name->getCString());
	}

	JSObjectRef callback = JSValueToObject(ctx, callbackValue, NULL);
	if (callback && JSObjectIsFunction(ctx, callback)) {
		JSValueProtect(ctx, callback);
		onCallbacks->setObject(new NSValue(callback, kJSObjectRef), name->getCString());
		return;
	}
}
//
EJ_BIND_FUNCTION(EJBindingEventedBase,addEventListener, ctx, argc, argv) {
	if (argc < 2) {
		return NULL;
	}

	NSString * name = JSValueToNSString(ctx, argv[0]);
	JSObjectRef callback = JSValueToObject(ctx, argv[1], NULL);
	JSValueProtect(ctx, callback);
	NSValue * callbackValue = new NSValue(callback, kJSObjectRef);

	NSArray * listeners = NULL;
	if ((listeners = (NSArray *) eventListeners->objectForKey(
			name->getCString()))) {
		listeners->addObject(callbackValue);
	} else {
		eventListeners->setObject(NSArray::createWithObject(callbackValue),
				name->getCString());
	}
	return NULL;
}
//
EJ_BIND_FUNCTION(EJBindingEventedBase,removeEventListener, ctx, argc, argv) {
	if( argc < 2 ) { return NULL; }

	NSString * name = JSValueToNSString( ctx, argv[0] );

	NSArray * listeners = NULL;
	if( (listeners = (NSArray *)eventListeners->objectForKey(name->getCString()) ) ) {
		JSObjectRef callback = JSValueToObject(ctx, argv[1], NULL);
		for(unsigned int i = 0; i < listeners->count(); i++ ) {
			NSValue* temp =  (NSValue *)listeners->objectAtIndex(i);
			if( JSValueIsStrictEqual(ctx, callback, (JSObjectRef)temp->pointerValue() ) ) {
				listeners->removeObjectAtIndex(i);
				return NULL;
			}
		}
	}
	return NULL;
}

void EJBindingEventedBase::triggerEvent(NSString* eventType)
{
	triggerEvent(eventType, 0, (JSEventProperty*)NULL);
}

void EJBindingEventedBase::triggerEvent(NSString * eventType, int argc, JSEventProperty properties[])
{
	EJApp * ejecta = EJApp::instance();

	NSArray * listeners = (NSArray *) eventListeners->objectForKey( eventType->getCString() );

	// Build the event object
	JSObjectRef jsEvent = EJBindingEvent::createJSObjectWithContext(EJApp::instance()->jsGlobalContext, eventType, jsObject);

	// Attach all additional properties, if any

	for( int i = 0; i<argc; i++ ) {
		JSStringRef name = JSStringCreateWithUTF8CString(properties[i].name);
		JSValueRef value = properties[i].value;
		JSObjectSetProperty(EJApp::instance()->jsGlobalContext, jsEvent, name, value, kJSPropertyAttributeReadOnly, NULL);
		JSStringRelease(name);
	}

	JSValueRef params[] = { jsEvent };
	if( listeners )
	{
		for (unsigned int var = 0; var < listeners->count(); ++var) {
			NSValue * callbackValue = (NSValue *) listeners->objectAtIndex(var);
			ejecta->invokeCallback((JSObjectRef)callbackValue->pointerValue(), jsObject, 1, params);
		}
	}

	NSValue * callbackValue = (NSValue *) onCallbacks->objectForKey(eventType->getCString());
	if (callbackValue) {
		ejecta->invokeCallback((JSObjectRef)callbackValue->pointerValue(), jsObject, 1, params);
	}
}

//
void EJBindingEventedBase::triggerEvent(NSString * name, int argc,
		JSValueRef argv[]) {
	EJApp * ejecta = EJApp::instance();

	NSArray * listeners = (NSArray *) eventListeners->objectForKey(
			name->getCString());
	if (listeners) {

		for (unsigned int var = 0; var < listeners->count(); ++var) {
			NSValue * callbackValue = (NSValue *) listeners->objectAtIndex(var);
			ejecta->invokeCallback((JSObjectRef)callbackValue->pointerValue(), jsObject,
					argc, argv);
		}
	}

	NSValue * callbackValue = (NSValue *) onCallbacks->objectForKey(
			name->getCString());
	if (callbackValue) {
		ejecta->invokeCallback((JSObjectRef)callbackValue->pointerValue(), jsObject, argc,
				argv);
	}
}

REFECTION_CLASS_IMPLEMENT(EJBindingEventedBase);

EJBindingEvent::EJBindingEvent()
:type(NULL), jsTarget(NULL)
{
}

EJBindingEvent::~EJBindingEvent()
{
	JSContextRef ctx = EJApp::instance()->jsGlobalContext;
	if(type)
	{
		type->release();
		JSValueUnprotect(ctx, jsTarget);
	}
}

JSObjectRef EJBindingEvent::createJSObjectWithContext(JSContextRef ctx, NSString* pType, JSObjectRef target)
{
	EJBindingEvent* event = new EJBindingEvent();
	event->initWithContext(ctx, NULL, 0, NULL);
	JSValueProtect(ctx, target);
	event->jsTarget = target;
	pType->retain();
	event->type = pType;

	return EJBindingBase::createJSObjectWithContext(ctx, event);
}


EJ_BIND_GET(EJBindingEvent, target, ctx)
{
	return jsTarget;
}

EJ_BIND_GET(EJBindingEvent, currentTarget, ctx )
{
	return jsTarget;
}

EJ_BIND_GET(EJBindingEvent, type, ctx )
{
	return NSStringToJSValue(ctx, type);
}


REFECTION_CLASS_IMPLEMENT(EJBindingEvent);

