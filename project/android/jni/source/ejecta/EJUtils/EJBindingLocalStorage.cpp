#include "EJBindingLocalStorage.h"

void EJBindingLocalStorage::initWithContext(JSContextRef ctxp, JSObjectRef obj, size_t argc, const JSValueRef argv[]) {
	_g_obj = EJApp::instance()->g_obj;

	env = NULL;
	EJApp::instance()->jvm->GetEnv((void**)&env, JNI_VERSION_1_6);

	jclass tmp  = env->GetObjectClass(_g_obj);
	javaCallerClass = (jclass)env->NewGlobalRef(tmp);

	env->DeleteLocalRef(tmp);
}

EJBindingLocalStorage::EJBindingLocalStorage() {
}

EJBindingLocalStorage::~EJBindingLocalStorage() {
}

EJ_BIND_FUNCTION(EJBindingLocalStorage, getItem, ctx, argc, argv ) {
	if( argc < 1 ) return NULL;

	NSString * key = JSValueToNSString( ctx, argv[0] );
	char * val;

	jmethodID methodId_getSharedPref = env->GetMethodID(
		javaCallerClass,
		"getSharedPreferences",
		"(Ljava/lang/String;)Ljava/lang/String;");

	jstring pkey = env->NewStringUTF(key->getCString()); 

	jstring my_java_string = (jstring) env->CallObjectMethod(_g_obj, methodId_getSharedPref, pkey);
	val = strdup(env->GetStringUTFChars(my_java_string, 0));

	env->DeleteLocalRef(pkey);

	return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(val));
}

EJ_BIND_FUNCTION(EJBindingLocalStorage, setItem, ctx, argc, argv ) {
	if( argc < 2 ) return NULL;

	NSString * key = JSValueToNSString( ctx, argv[0] );
	NSString * value = JSValueToNSString( ctx, argv[1] );

	jmethodID methodId_setSharedPref = env->GetMethodID(
		javaCallerClass,
		"setSharedPreferences",
		"(Ljava/lang/String;Ljava/lang/String;)V");

	jstring pkey = env->NewStringUTF(key->getCString()); 
	jstring pvalue = env->NewStringUTF(value->getCString()); 

	env->CallVoidMethod(_g_obj, methodId_setSharedPref, pkey, pvalue);

	env->DeleteLocalRef(pkey);
	env->DeleteLocalRef(pvalue);

	return NULL;
}

EJ_BIND_FUNCTION(EJBindingLocalStorage, removeItem, ctx, argc, argv ) {
	if( argc < 1 ) return NULL;
	
	NSString * key = JSValueToNSString( ctx, argv[0] );
	
	jmethodID methodId_removeSharedPref = env->GetMethodID(
		javaCallerClass,
		"removeSharedPreferences",
		"(Ljava/lang/String;)V");

	jstring pkey = env->NewStringUTF(key->getCString()); 

	env->CallVoidMethod(_g_obj, methodId_removeSharedPref, pkey);

	env->DeleteLocalRef(pkey);
	
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingLocalStorage, clear, ctx, argc, argv ) {
	jmethodID methodId_resetSharedPref = env->GetMethodID(
		javaCallerClass,
		"resetSharedPreferences",
		"()V");

	env->CallVoidMethod(_g_obj, methodId_resetSharedPref);
	return NULL;
}

REFECTION_CLASS_IMPLEMENT(EJBindingLocalStorage);
