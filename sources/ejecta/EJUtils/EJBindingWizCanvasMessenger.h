#ifndef __EJ_BINDING_WIZCANVASMESSENGER_H__
#define __EJ_BINDING_WIZCANVASMESSENGER_H__

#include "../EJBindingEventedBase.h"

class EJBindingWizCanvasMessenger : public EJBindingEventedBase {
    
        NSString* viewName;
        
public:       
	
        EJBindingWizCanvasMessenger();
        EJBindingWizCanvasMessenger(const char* name);
        ~EJBindingWizCanvasMessenger();
        
        virtual void init(JSContextRef ctx ,JSObjectRef obj, size_t argc, const JSValueRef argv[]);

	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingWizCanvasMessenger);

	virtual string superclass(){ return EJBindingEventedBase::toString();};

        void triggerEvent(NSString* script);
        
	EJ_BIND_FUNCTION_DEFINE(postMessage, ctx, argc, argv );
        EJ_BIND_FUNCTION_DEFINE(__triggerMessageEvent, ctx, argc, argv );
};

#endif // __EJ_BINDING_WIZCANVASMESSENGER_H__
