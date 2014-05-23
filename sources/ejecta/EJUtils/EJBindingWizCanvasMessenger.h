#ifndef __EJ_BINDING_WIZCANVASMESSENGER_H__
#define __EJ_BINDING_WIZCANVASMESSENGER_H__

#include "../EJBindingEventedBase.h"

class EJMessageEvent : public NSObject {
public:
	EJMessageEvent() {
        eventName = new NSString("");
        message = new NSString("");
        type = new NSString("");
    };
    EJMessageEvent(const char *theName, const char *theMessage, const char *theType) {
        // Construct the object and convert the chars to 
        // NSString accessor properties
        eventName = new NSString(theName);
        message = new NSString(theMessage);
        type = new NSString(theType);
    };
    ~EJMessageEvent() {
        eventName->release();
        message->release();
        type->release();
    };

    NSString *eventName;
    NSString *message;
    NSString *type;
};

class EJBindingWizCanvasMessenger: public EJBindingEventedBase {
    
    NSString *viewName;
        
public:       

    EJBindingWizCanvasMessenger();
    EJBindingWizCanvasMessenger(const char *name);
    ~EJBindingWizCanvasMessenger();

    virtual void initWithContext(JSContextRef ctx, JSObjectRef obj, size_t argc, const JSValueRef argv[]);

    REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingWizCanvasMessenger);

    virtual string superclass() { return EJBindingEventedBase::toString(); };

    void triggerEvent(NSString *name, NSString *message, NSString *type);

	EJ_BIND_FUNCTION_DEFINE(postMessage, ctx, argc, argv );
    EJ_BIND_FUNCTION_DEFINE(__triggerMessageEvent, ctx, argc, argv);

    jmethodID receiverMethodId;
    JNIEnv *g_env;

};

#endif // __EJ_BINDING_WIZCANVASMESSENGER_H__
