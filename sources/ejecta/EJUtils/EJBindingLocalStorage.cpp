#include "EJBindingLocalStorage.h"

void EJBindingLocalStorage::init(JSContextRef ctx, JSObjectRef obj, size_t argc, const JSValueRef argv[]) {
    
    _g_obj = EJApp::instance()->g_obj;
    
    env = NULL;
    EJApp::instance()->jvm->GetEnv((void**)&env, JNI_VERSION_1_6);
    
    javaCallerClass = env->GetObjectClass(_g_obj);
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
	
	// NSString * key = JSValueToNSString( ctx, argv[0] );
	// [[NSUserDefaults standardUserDefaults] removeObjectForKey:key];
	
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingLocalStorage, clear, ctx, argc, argv ) {
	//[[NSUserDefaults standardUserDefaults] se"tPersistentDomain:[NSDictionary dictionary] forName:[[NSBundle mainBundle] bundleIdentifier]];
	return NULL;
}

REFECTION_CLASS_IMPLEMENT(EJBindingLocalStorage);
