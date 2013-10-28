#include "EJBindingWizCanvasMessenger.h"
#include "../EJCocoa/NSString.h"
#include <JavaScriptCore/JavaScriptCore.h>
#include "../EJBindingEventedBase.h"
#include "../EJApp.h"

void EJBindingWizCanvasMessenger::init(JSContextRef ctx ,JSObjectRef obj, size_t argc, const JSValueRef argv[]) {
    if (argc > 0) {
        viewName = JSValueToNSString( ctx, argv[0] ); viewName->retain();
        NSLog("init WizCanvasMessenger on %s", viewName->getCString());
    } else {
        NSLog("init no params");
        viewName =  NSStringMake("unknown");
    }
}

EJBindingWizCanvasMessenger::EJBindingWizCanvasMessenger() {
    EJApp::instance()->messengerDelegate = this;
    viewName = new NSString("newCanvas");
}

EJBindingWizCanvasMessenger::EJBindingWizCanvasMessenger(const char* name) {
    EJApp::instance()->messengerDelegate = this;
    viewName = new NSString(name);
}

EJBindingWizCanvasMessenger::~EJBindingWizCanvasMessenger() {
}

void EJBindingWizCanvasMessenger::triggerEvent(NSString* script) {
    EJBindingEventedBase::triggerEvent(NSStringMake("message"), 0, NULL);
}

EJ_BIND_FUNCTION(EJBindingWizCanvasMessenger, postMessage, ctx, argc, argv ) {
    NSLog("postMessage from Ejecta");
    if( argc < 2 ) {
        NSLog("WizViewMessenger Error : Not enough params supplied to wizViewMessenger.postMessage(<target>, <data>)");
        return NULL;
    }

    // JSType *js_type = JSValueGetType(ctx, argv[0]);

    // Check raw data type
    NSString *type = new NSString();
    if (JSValueIsBoolean(ctx, argv[0])) {
        NSLog("boolean");
        type = NSStringMake("Boolean");
    } else if (JSValueIsNumber(ctx, argv[0])) {
        NSLog("Number");
        type = NSStringMake("Number");
    } else if (JSValueIsString(ctx, argv[0])) {
        NSLog("String");
        type = NSStringMake("String");
    } else if (JSValueIsObject(ctx, argv[0])) {
        NSLog("Object");
        type = NSStringMake("Object");
    }

    // Stringify our data using JavaScriptCore
    JSStringRef scriptJS = JSStringCreateWithUTF8CString("return JSON.stringify( arguments[0] )");
    JSObjectRef fn = JSObjectMakeFunction(ctx, NULL, 0, NULL, scriptJS, NULL, 1, NULL);
    JSValueRef result = JSObjectCallAsFunction(ctx, fn, NULL, 1, argv, NULL);
    NSString *message = JSValueToNSString(ctx, result);
    NSString *targetName = JSValueToNSString( ctx, argv[1] );

    NSLog("Send to target view: %s", viewName->getCString());
    
    // Send our message to the target view
    JNIEnv *g_env;
    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose a JNI version
    args.name = NULL; // could name this java thread
    args.group = NULL; // could assign the java thread to a ThreadGroup

    JavaVM *_jvm = EJApp::instance()->jvm;
    if (_jvm == NULL) {
        NSLOG("ERROR JVM is NULL");
        return NULL;
    }
    // Double check it's all OK
    int getEnvStat = _jvm->GetEnv((void **)&g_env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        NSLOG("GetEnv: not attached");
        /*
        if (_jvm->AttachCurrentThread((void **) &g_env, NULL) != 0) {
            NSLOG("Failed to attach");
        }
        */
        return NULL;
    } else if (getEnvStat == JNI_OK) {
        // NSLOG("JNI OK");
    } else if (getEnvStat == JNI_EVERSION) {
        NSLOG("GetEnv: version not supported");
        return NULL;
    }
    jclass cls = g_env->FindClass("com/impactjs/ejecta/EjectaRenderer");
    if (g_env->ExceptionCheck()) {
        NSLOG("ERR_FIND_CLASS_FAILED");
        return NULL;
    }
    // Get method Id and set argument signature - String, String, String, Void return
    jmethodID mid = g_env->GetMethodID(cls, "onPostMessageReceived", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (g_env->ExceptionCheck()) {
        NSLOG("ERR_GET_METHOD_FAILED");
        return NULL;
    }
    jobject _g_obj = EJApp::instance()->g_obj;
    // Arguments must be jvalue
    jstring _targetName = g_env->NewStringUTF(targetName->getCString());
    jstring _message = g_env->NewStringUTF(message->getCString());
    jstring _type = g_env->NewStringUTF(type->getCString());
    jstring _viewName = g_env->NewStringUTF(viewName->getCString());
    g_env->CallVoidMethod(
        _g_obj, 
        mid, 
        _targetName, 
        _message, 
        _type, 
        _viewName);
    if (g_env->ExceptionCheck()) {
        NSLOG("ERR_CALL_METHOD_FAILED");
        return NULL;
    }
    // Remove the local reference, JNI will
    // free this memory heap automagically with garbage collector
    g_env->DeleteLocalRef(_targetName);
    g_env->DeleteLocalRef(_message);
    g_env->DeleteLocalRef(_type);
    g_env->DeleteLocalRef(_viewName);
    return NULL;        
}

EJ_BIND_FUNCTION(EJBindingWizCanvasMessenger, __triggerMessageEvent, ctx, argc, argv ) {
    NSLog("Trigger message event.");

    NSString *origin;
    NSString *targetView;
    NSString *data;
    NSString *type;
    if( argc > 3 ) {
        // Get args
        // 0: originView, 1: targetView, 2: postDataEscaped, 3:type

        origin = JSValueToNSString(ctx, argv[0]); origin->retain();
        targetView = JSValueToNSString(ctx, argv[1]); targetView->retain();
        data = JSValueToNSString(ctx, argv[2]); data->retain();
        type = JSValueToNSString(ctx, argv[3]); type->retain();
        // NSLog("type: %s", type->getCString());
    } else {
        NSLog("WizCanvasMessenger for Ejecta - Error missing arguments in postMessage");
        return NULL;
    }
    /*   
    NSLog("Sending message into Ejecta");
    string typeString = string(type->getCString());

    JSObjectRef eventObject = JSObjectMake( ctx, NULL, NULL );
   
    if( typeString == "Array" ) {
        NSLog("Message as Array");
        JSValueRef arrayData = JSValueMakeFromJSONString(ctx, JSStringCreateWithUTF8CString(data->getCString()));

        // void triggerEvent(NSString * name, int argc, JSValueRef argv[]);
        // JSObjectSetProperty(ctx, objRef, stringRef, JSValueMakeNumber(ctx, stringWidth), kJSPropertyAttributeNone, NULL);
		JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("data"), arrayData, 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("origin"), NSStringToJSValue(ctx, origin), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("source"), JSValueMakeBoolean(ctx, targetView), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                
        EJBindingEventedBase::triggerEvent(NSStringMake("message"), 1, (JSValueRef*)&eventObject);

    } else if( typeString == "String" ) {
        NSLog("Message as String");
        
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("data"), NSStringToJSValue(ctx, data), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("origin"), NSStringToJSValue(ctx, origin), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("source"), JSValueMakeBoolean(ctx, targetView), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
        EJBindingEventedBase::triggerEvent(NSStringMake("message"), 1, (JSValueRef*)&eventObject);

    } else if( typeString == "Number" ) {
        NSLog("Message as Number");

                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("data"), argv[2], 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("origin"), NSStringToJSValue(ctx, origin), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("source"), JSValueMakeBoolean(ctx, targetView), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
        EJBindingEventedBase::triggerEvent(NSStringMake("message"), 1, (JSValueRef*)&eventObject);
        
    } else if( typeString == "Boolean" ) {
        NSLog("Message as Boolean");
        
        JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("data"), JSValueMakeBoolean(ctx, data), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("origin"), NSStringToJSValue(ctx, origin), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("source"), JSValueMakeBoolean(ctx, targetView), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
        EJBindingEventedBase::triggerEvent(NSStringMake("message"), 1, (JSValueRef*)&eventObject);

    } else if( typeString == "Function" ) {
        NSLog("Message as Function");
        // W3C says nothing about functions, will be returned as String type.
        
        JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("data"), NSStringToJSValue(ctx, data), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("origin"), NSStringToJSValue(ctx, origin), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("source"), JSValueMakeBoolean(ctx, targetView), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
        EJBindingEventedBase::triggerEvent(NSStringMake("message"), 1, (JSValueRef*)&eventObject);

    } else if( typeString == "Object" ) {
        NSLog("Message as Object");
        JSValueRef objData = JSValueMakeFromJSONString(ctx, JSStringCreateWithUTF8CString(data->getCString()));
        
        JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("data"), objData, 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("origin"), NSStringToJSValue(ctx, origin), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
                JSObjectSetProperty(
			ctx, eventObject, 
			JSStringCreateWithUTF8CString("source"), JSValueMakeBoolean(ctx, targetView), 
			kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, NULL
		);
        
//      EJBindingEventedBase::triggerEvent(NSStringMake("message"), 1, (JSValueRef*)&eventObject);
//      EJBindingEventedBase::triggerEvent(NSStringMake("message"), 0, NULL);
        
    }
    */
    /*
    string js = "wizCanvasMessenger.receiver({ source: 'newCanvas', origin:'mainView', data: JSON.parse(decodeURIComponent('";
    js.append(data->getCString());
    js.append("')) });");
    // char *jsChar = (char*)js.c_str();
    // NSLog("Trigger event: %s", jsChar);
    // const char *scriptAsChar = (env)->GetStringUTFChars(script, 0);
    //EJApp::instance()->evaluateScript(jsChar);
    
    NSString *convertedscript = NSStringMake(js);
    
    JSStringRef scriptJS = JSStringCreateWithUTF8CString(convertedscript->getCString());
        
    // JSValueRef exception = NULL;
    //EJApp::instance()->jsGlobalContext
    JSEvaluateScript( EJApp::instance()->jsGlobalContext, scriptJS, NULL, NULL, 0, NULL );
    // logException(exception, EJApp::instance()->jsGlobalContext);

    // JSStringRelease( scriptJS );
       */ 
    EJBindingEventedBase::triggerEvent(NSStringMake("message"), 0, NULL);
    
    origin->release();
    targetView->release();
    data->release();
    type->release();
    
    return NULL;
}

EJ_BIND_EVENT(EJBindingWizCanvasMessenger, Message);

REFECTION_CLASS_IMPLEMENT(EJBindingWizCanvasMessenger);
