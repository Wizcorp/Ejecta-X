#include "EJBindingLocalStorage.h"

EJBindingLocalStorage::EJBindingLocalStorage() {
}

EJBindingLocalStorage::~EJBindingLocalStorage() {
}

EJ_BIND_FUNCTION(EJBindingLocalStorage, getItem, ctx, argc, argv ) {
	if( argc < 1 ) return NULL;

	NSString * key = JSValueToNSString( ctx, argv[0] );
	char * val;

	JavaVM* g_JavaVM = EJApp::instance()->getJvm();
	JNIEnv *env = NULL;
	g_JavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
	if (env == NULL)
	    return NULL;

	jobject jobj = EJApp::instance()->getJobj();
	jclass javaCallerClass = env->GetObjectClass(jobj);

	jmethodID methodId_getSharedPref = env->GetMethodID(
	 javaCallerClass,
	 "getSharedPreferences",
	 "(Ljava/lang/String;)Ljava/lang/String;");

	jstring pkey = env->NewStringUTF(key->getCString()); 

	jstring my_java_string = (jstring) env->CallObjectMethod(jobj, methodId_getSharedPref, pkey);
	val = strdup(env->GetStringUTFChars(my_java_string, 0));

	return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(val));
}

EJ_BIND_FUNCTION(EJBindingLocalStorage, setItem, ctx, argc, argv ) {
	if( argc < 2 ) return NULL;

	NSString * key = JSValueToNSString( ctx, argv[0] );
	NSString * value = JSValueToNSString( ctx, argv[1] );

	JavaVM* g_JavaVM = EJApp::instance()->getJvm();
	JNIEnv *env = NULL;
	g_JavaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
	if (env == NULL)
	    return NULL;

	jobject jobj = EJApp::instance()->getJobj();
	jclass javaCallerClass = env->GetObjectClass(jobj);

	jmethodID methodId_setSharedPref = env->GetMethodID(
	 javaCallerClass,
	 "setSharedPreferences",
	 "(Ljava/lang/String;Ljava/lang/String;)V");

	jstring pkey = env->NewStringUTF(key->getCString()); 
	jstring pvalue = env->NewStringUTF(value->getCString()); 

	env->CallVoidMethod(jobj, methodId_setSharedPref, pkey, pvalue);

	return NULL;
}

EJ_BIND_FUNCTION(EJBindingLocalStorage, removeItem, ctx, argc, argv ) {
	if( argc < 1 ) return NULL;
	
	// NSString * key = JSValueToNSString( ctx, argv[0] );
	// [[NSUserDefaults standardUserDefaults] removeObjectForKey:key];
	
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingLocalStorage, clear, ctx, argc, argv ) {
	//[[NSUserDefaults standardUserDefaults] setPersistentDomain:[NSDictionary dictionary] forName:[[NSBundle mainBundle] bundleIdentifier]];
	return NULL;
}

REFECTION_CLASS_IMPLEMENT(EJBindingLocalStorage);
