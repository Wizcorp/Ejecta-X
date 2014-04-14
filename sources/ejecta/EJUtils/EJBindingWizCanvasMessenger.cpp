#include "EJBindingWizCanvasMessenger.h"
#include "../EJCocoa/NSString.h"
#include <JavaScriptCore/JavaScriptCore.h>

void EJBindingWizCanvasMessenger::init(JSContextRef ctx, JSObjectRef obj, size_t argc, const JSValueRef argv[]) {
    if (argc > 0) {
        viewName = JSValueToNSString(ctx, argv[0]);
        viewName->retain();
        NSLog("init WizCanvasMessenger on %s", viewName->getCString());
    } else {
        NSLog("init no params");
        viewName = NSStringMake("unknown");
    }
    EJApp::instance()->messengerDelegate = this;
    
    // Set up access to Java Class and MethodID
    // Send our message to the target view
    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // choose a JNI version
    args.name = NULL; // could name this java thread
    args.group = NULL; // could assign the java thread to a ThreadGroup

    JavaVM *_jvm = EJApp::instance()->jvm;
    if (_jvm == NULL) {
        NSLOG("ERROR JVM is NULL");
        return;
    }
    // Double check it's all OK
    int getEnvStat = _jvm->GetEnv((void **)&g_env, JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        NSLOG("GetEnv: not attached");
        return;
    } else if (getEnvStat == JNI_OK) {
        // NSLOG("JNI OK");
    } else if (getEnvStat == JNI_EVERSION) {
        NSLOG("GetEnv: version not supported");
        return;
    }
    jclass receiverClass = g_env->FindClass("com/impactjs/ejecta/EjectaRenderer");
    if (g_env->ExceptionCheck()) {
        NSLOG("ERR_FIND_CLASS_FAILED");
        return;
    }
    // Get method Id and set argument signature - String, String, String, Void return
    receiverMethodId = g_env->GetMethodID(receiverClass, "onPostMessageReceived", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (g_env->ExceptionCheck()) {
        NSLOG("ERR_GET_METHOD_FAILED");
        return;
    }
}

EJBindingWizCanvasMessenger::EJBindingWizCanvasMessenger() {
    EJApp::instance()->messengerDelegate = this;
    viewName = new NSString("unknown");
}

EJBindingWizCanvasMessenger::EJBindingWizCanvasMessenger(const char* name) {
    // NSLog("init WizCanvasMessenger constructor: %s", name);
    EJApp::instance()->messengerDelegate = this;
    viewName = new NSString(name);
}

EJBindingWizCanvasMessenger::~EJBindingWizCanvasMessenger() {
}

void EJBindingWizCanvasMessenger::triggerEvent(NSString *name, NSString *message, NSString *type) {
    EJApp *ejecta = EJApp::instance();
    JSContextRef ctx = ejecta->jsGlobalContext;
          
    NSString *origin = NSStringMake("mainView");
    NSString *target = NSStringMake(viewName->getCString());
    
    JSStringRef origin_string = JSStringCreateWithUTF8CString(origin->getCString()); 
    JSValueRef _origin = JSValueMakeString(ctx, origin_string);
    
    JSStringRef target_string = JSStringCreateWithUTF8CString(target->getCString()); 
    JSValueRef _target = JSValueMakeString(ctx, target_string);
    
    JSStringRef message_string = JSStringCreateWithUTF8CString(message->getCString()); 
    JSValueRef _message = JSValueMakeString(ctx, message_string);
    
    JSStringRef type_string = JSStringCreateWithUTF8CString(type->getCString()); 
    JSValueRef _type = JSValueMakeString(ctx, type_string);

    JSValueRef params[] = {_origin, _target, _message, _type};
    
    //param[1] = result;
    EJBindingEventedBase::triggerEvent(name, 4, params);
}

EJ_BIND_FUNCTION(EJBindingWizCanvasMessenger, postMessage, ctx, argc, argv ) {
    if( argc < 2 ) {
        NSLog("WizViewMessenger Error : Not enough params supplied to wizViewMessenger.postMessage(<target>, <data>)");
        return NULL;
    }
    // JSType *js_type = JSValueGetType(ctx, argv[0]);

    // Check raw data type
    NSString *type = new NSString();
    if (JSValueIsBoolean(ctx, argv[0])) {
        // NSLog("boolean");
        type = NSStringMake("Boolean");
    } else if (JSValueIsNumber(ctx, argv[0])) {
        // NSLog("Number");
        type = NSStringMake("Number");
    } else if (JSValueIsString(ctx, argv[0])) {
        // NSLog("String");
        type = NSStringMake("String");
    } else if (JSValueIsObject(ctx, argv[0])) {
        // NSLog("Object");
        type = NSStringMake("Object");
    }

    // Stringify our data using JavaScriptCore
    JSStringRef scriptJS = JSStringCreateWithUTF8CString("return JSON.stringify(arguments[0])");
    JSObjectRef fn = JSObjectMakeFunction(ctx, NULL, 0, NULL, scriptJS, NULL, 1, NULL);
    JSValueRef result = JSObjectCallAsFunction(ctx, fn, NULL, 1, argv, NULL);
    NSString *message = JSValueToNSString(ctx, result);
    NSString *targetName = JSValueToNSString(ctx, argv[1]);

    // NSLog("Send to target view: %s", viewName->getCString());

    jobject _g_obj = EJApp::instance()->g_obj;
    
    // Arguments must be jstring
    jstring _targetName = g_env->NewStringUTF(targetName->getCString());
    jstring _message = g_env->NewStringUTF(message->getCString());
    jstring _type = g_env->NewStringUTF(type->getCString());
    jstring _viewName = g_env->NewStringUTF(viewName->getCString());
    g_env->CallVoidMethod(
        _g_obj, 
        receiverMethodId, 
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

EJ_BIND_EVENT(EJBindingWizCanvasMessenger, message);

REFECTION_CLASS_IMPLEMENT(EJBindingWizCanvasMessenger);
