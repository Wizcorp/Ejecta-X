#include "EJClassLoader.h"
#include "EJBindingBase.h"
#include "EJJavaScriptView.h"


static JSClassRef EJGlobalConstructorClass;
static NSDictionary *EJGlobalJSClassCache;

typedef struct {
	EJBindingBase* classId;
	EJJavaScriptView *scriptView;
} EJClassWithScriptView;

JSValueRef EJGetNativeClass(JSContextRef ctx, JSObjectRef object, JSStringRef propertyNameJS, JSValueRef* exception) 
{
	size_t classNameSize = JSStringGetMaximumUTF8CStringSize(propertyNameJS);
    char* className = (char*)malloc(classNameSize);
    JSStringGetUTF8CString(propertyNameJS, className, classNameSize);
    EJJavaScriptView *scriptView = JSObjectGetPrivate(object);
	
 	JSObjectRef obj = NULL;
 	NSString * fullClassName = new NSString();
 	fullClassName->initWithFormat("EJBinding%s",className);

 	EJBindingBase* pClass = (EJBindingBase*)NSClassFromString(fullClassName->getCString());

	if( pClass ) {
		
		// Pack the class together with the scriptView into a struct, so it can
		// be put in the constructor's private data
		EJClassWithScriptView *classWithScriptView = malloc(sizeof(EJClassWithScriptView));
		classWithScriptView->classId = pClass;
		classWithScriptView->scriptView = scriptView;
		
		obj = JSObjectMake( ctx, EJGlobalConstructorClass, (void *)classWithScriptView );
	}
	
    free(className);
    fullClassName->autorelease();
 	return obj ? obj : scriptView->jsUndefined;
}

JSObjectRef EJCallAsConstructor(JSContextRef ctx, JSObjectRef constructor, size_t argc, const JSValueRef argv[], JSValueRef* exception) {
	
	// Unpack the class and scriptView from the constructor's private data
	EJClassWithScriptView *classWithScriptView = (EJClassWithScriptView *)JSObjectGetPrivate(constructor);
	EJBindingBase* pClass = classWithScriptView->classid;
	EJJavaScriptView *scriptView = classWithScriptView->scriptView;
	
	// Init the native class and create the JSObject with it
	EJBindingBase *instance = (EJBindingBase*)NSClassFromString(pClass->toString().c_str());
	instance->init(ctx, argc, argv);
	JSObjectRef obj = pClass->createJSObject(ctx, scriptView, instance);
	instance->release();
	
	return obj;
}

void EJConstructorFinalize(JSObjectRef object) {
	EJClassWithScriptView *classWithScriptView = (EJClassWithScriptView *)JSObjectGetPrivate(object);
	free(classWithScriptView);
}

bool EJConstructorHasInstance(JSContextRef ctx, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception) {

	// Unpack the class and instance from private data
	EJClassWithScriptView *classWithScriptView = (EJClassWithScriptView *)JSObjectGetPrivate(constructor);
	EJClassWithScriptView *instance = JSObjectGetPrivate((JSObjectRef)possibleInstance);
	
	if( !classWithScriptView || !instance ) {
		return false;
	}
	
	return true;//[instance isKindOfClass:classWithScriptView->class];
}



EJClassLoader::EJClassLoader(EJJavaScriptView *scriptView, NSString *name)
{
	JSGlobalContextRef context = scriptView.jsGlobalContext;
		
	// Create or retain the global constructor class
	if( !EJGlobalConstructorClass ) {
		JSClassDefinition constructorClassDef = kJSClassDefinitionEmpty;
		constructorClassDef.callAsConstructor = EJCallAsConstructor;
		constructorClassDef.hasInstance = EJConstructorHasInstance;
		constructorClassDef.finalize = EJConstructorFinalize;
		EJGlobalConstructorClass = JSClassCreate(&constructorClassDef);
	}
	else {
		JSClassRetain(EJGlobalConstructorClass);
	}
	
	// Create the collection class and attach it to the global context with
	// the given name
	JSClassDefinition loaderClassDef = kJSClassDefinitionEmpty;
	loaderClassDef.getProperty = EJGetNativeClass;
	JSClassRef loaderClass = JSClassCreate(&loaderClassDef);
	
	JSObjectRef global = JSContextGetGlobalObject(context);
	
	JSObjectRef loader = JSObjectMake(context, loaderClass, scriptView);
	JSStringRef jsName = JSStringCreateWithUTF8CString(name->getCString());
	JSObjectSetProperty(
		context, global, jsName, loader,
		(kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly),
		NULL
	);
	JSStringRelease(jsName);
	
	
	// Create or retain the global Class cache
	if( !EJGlobalJSClassCache ) {
		EJGlobalJSClassCache = NSDictionary::create();
	}
	else {
		EJGlobalJSClassCache->retain();
	}
}

EJClassLoader::~EJClassLoader();
	// If we are the last Collection to hold on to the Class cache, release it and
	// set it to nil, so it can be properly re-created if needed.
	if( EJGlobalJSClassCache->retainCount() == 1 ) {
		EJGlobalJSClassCache->release();
		EJGlobalJSClassCache = NULL;
	}
	else {
		EJGlobalJSClassCache->release();
	}
	
	JSClassRelease(EJGlobalConstructorClass);
}

JSClassRef EJClassLoader::getJSClass(EJBindingBase* classId)
{
	NSAssert(EJGlobalJSClassCache != NULL, "Attempt to access class cache without a loader present.");
	
	// Try the cache first
	JSClassRef jsClass = NULL;
	if (EJGlobalJSClassCache->count())
	{

		NSDictElement* pElement = NULL;
		NSObject* pObject = NULL;
		NSDICT_FOREACH(EJGlobalJSClassCache, pElement)
		{
			string key = string(pElement->getStrKey());
	        if( key == classId->toString() ) {
				jsClass = (JSClassRef)((NSValue*)EJGlobalJSClassCache->objectForKey(classId->toString()))->pointerValue();
				break;
			}	
		}
	}
	if( jsClass ) {
		return jsClass;
	}
	
	// Still here? Create and insert into cache
	jsClass = createJSClass(classId);
	EJGlobalJSClassCache->setObject(new NSValue(jsClass, kJSClassRef), classId->toString());
	return jsClass;
}

JSClassRef EJBindingBase::createJSClass (EJBindingBase* ej_obj){
	// Gather all class methods that return C callbacks for this class or it's parents
	NSDictionary * methods = NSDictionary::create();
	NSDictionary * properties = NSDictionary::create();

	string base_obj = ej_obj->toString();

	NSObjectFactory::fuc_map_type* base = NSObjectFactory::getFunctionMap();
	for(NSObjectFactory::fuc_map_type::iterator it = base->begin(); it != base->end(); it++)
	{
		string name = it->first;
		string base_obj_tmp = base_obj;
		if( name.find("_func_") != string::npos ) {
			int pos = name.find("_func_");
			int is_member_func = name.find(base_obj_tmp);
			bool n_pos = (is_member_func == string::npos);
			while(n_pos) {
				EJBindingBase* pClass = (EJBindingBase*)NSClassFromString(base_obj_tmp);
				base_obj_tmp = pClass->superclass();
				is_member_func = name.find(base_obj_tmp);
				if ((is_member_func != string::npos) || (base_obj_tmp.find("EJBindingBase") != string::npos))
				{
					n_pos = false;
				}
			}
			if (is_member_func != string::npos)
			{
			   	methods->setObject(NSStringMake(name.substr(pos + strlen("_func_"))), name);
			}
		}
		else if( name.find("_get_") != string::npos ) {
			int pos = name.find("_get_");
			int is_member_func = name.find(base_obj_tmp);
			bool n_pos = (is_member_func == string::npos);
			while(n_pos) {
				EJBindingBase* pClass = (EJBindingBase*)NSClassFromString(base_obj_tmp);
				base_obj_tmp = pClass->superclass();
				is_member_func = name.find(base_obj_tmp);
				if ((is_member_func != string::npos) || (base_obj_tmp.find("EJBindingBase") != string::npos))
				{
					n_pos = false;
				}
			}
			if (is_member_func != string::npos)
			{
				// We only look for getters - a property that has a setter, but no getter will be ignored
			  	properties->setObject(NSStringMake(name.substr(pos + strlen("_get_"))), name);
			}
		}
	} 
	
	
	// Set up the JSStaticValue struct array
	JSStaticValue * values = (JSStaticValue *)calloc( properties->count() + 1, sizeof(JSStaticValue) );
	
	int i = 0;
	NSDictElement* pElement = NULL;
	NSObject* pObject = NULL;

	NSDICT_FOREACH(properties, pElement)
	{
		pObject = pElement->getObject()->copy();
		string key_name = pElement->getStrKey();
	    NSString* name = (NSString*)pObject;

		char * nameData = NSDataFromString( name );
		
		char** p_name = const_cast<char**>(&values[i].name);
		*p_name = nameData;
		values[i].attributes = kJSPropertyAttributeDontDelete;
		SEL get = NSSelectorFromString(key_name);
		values[i].getProperty = (JSObjectGetPropertyCallback)get;
		
		// Property has a setter? Otherwise mark as read only
		int pos = key_name.find("_get_");
		key_name = key_name.replace(pos, strlen("_get_"), "_set_");
		SEL set = NSSelectorFromString(key_name);
		if( set ) {
			values[i].setProperty = (JSObjectSetPropertyCallback)set;
		}
		else {
			values[i].attributes |= kJSPropertyAttributeReadOnly;
		}

		i++;
	}
	
	// Set up the JSStaticFunction struct array
	JSStaticFunction * functions = (JSStaticFunction *)calloc( methods->count() + 1, sizeof(JSStaticFunction) );

	i = 0;
	pElement = NULL;
	pObject = NULL;
	NSDICT_FOREACH(methods, pElement)
	{

		pObject = pElement->getObject()->copy();
		string key_name = pElement->getStrKey();
	    NSString* name = (NSString*)pObject;

		char * nameData = NSDataFromString( name );

		char** p_name = const_cast<char**>(&functions[i].name);
		*p_name = nameData;
		functions[i].attributes = kJSPropertyAttributeDontDelete;
		
		SEL call = NSSelectorFromString(key_name);
		functions[i].callAsFunction = (JSObjectCallAsFunctionCallback)call;

		i++;
	}
	
	JSClassDefinition classDef = kJSClassDefinitionEmpty;
	classDef.finalize = _ej_class_finalize;
	classDef.staticValues = values;
	classDef.staticFunctions = functions;
	JSClassRef js_class = JSClassCreate(&classDef);
	
	free( values );
	free( functions );
	
	properties->release();
	methods->release();

	return js_class;
}


// #import "EJClassLoader.h"
// #import "EJBindingBase.h"
// #import "EJJavaScriptView.h"


// static JSClassRef EJGlobalConstructorClass;
// static NSMutableDictionary *EJGlobalJSClassCache;

// typedef struct {
// 	Class class;
// 	EJJavaScriptView *scriptView;
// } EJClassWithScriptView;

// JSValueRef EJGetNativeClass(JSContextRef ctx, JSObjectRef object, JSStringRef propertyNameJS, JSValueRef* exception) {
// 	CFStringRef className = JSStringCopyCFString( kCFAllocatorDefault, propertyNameJS );
// 	EJJavaScriptView *scriptView = JSObjectGetPrivate(object);
	
// 	JSObjectRef obj = NULL;
// 	NSString *fullClassName = [@EJ_BINDING_CLASS_PREFIX stringByAppendingString:(NSString *)className];
// 	Class class = NSClassFromString(fullClassName);
	
// 	if( class && [class isSubclassOfClass:EJBindingBase.class] ) {
		
// 		// Pack the class together with the scriptView into a struct, so it can
// 		// be put in the constructor's private data
// 		EJClassWithScriptView *classWithScriptView = malloc(sizeof(EJClassWithScriptView));
// 		classWithScriptView->class = class;
// 		classWithScriptView->scriptView = scriptView;
		
// 		obj = JSObjectMake( ctx, EJGlobalConstructorClass, (void *)classWithScriptView );
// 	}
	
// 	CFRelease(className);
// 	return obj ? obj : scriptView->jsUndefined;
// }

// JSObjectRef EJCallAsConstructor(JSContextRef ctx, JSObjectRef constructor, size_t argc, const JSValueRef argv[], JSValueRef* exception) {
	
// 	// Unpack the class and scriptView from the constructor's private data
// 	EJClassWithScriptView *classWithScriptView = (EJClassWithScriptView *)JSObjectGetPrivate(constructor);
// 	Class class = classWithScriptView->class;
// 	EJJavaScriptView *scriptView = classWithScriptView->scriptView;
	
// 	// Init the native class and create the JSObject with it
// 	EJBindingBase *instance = [(EJBindingBase *)[class alloc] initWithContext:ctx argc:argc argv:argv];
// 	JSObjectRef obj = [class createJSObjectWithContext:ctx scriptView:scriptView instance:instance];
// 	[instance release];
	
// 	return obj;
// }

// void EJConstructorFinalize(JSObjectRef object) {
// 	EJClassWithScriptView *classWithScriptView = (EJClassWithScriptView *)JSObjectGetPrivate(object);
// 	free(classWithScriptView);
// }

// bool EJConstructorHasInstance(JSContextRef ctx, JSObjectRef constructor, JSValueRef possibleInstance, JSValueRef* exception) {

// 	// Unpack the class and instance from private data
// 	EJClassWithScriptView *classWithScriptView = (EJClassWithScriptView *)JSObjectGetPrivate(constructor);
// 	id instance = JSObjectGetPrivate((JSObjectRef)possibleInstance);
	
// 	if( !classWithScriptView || !instance ) {
// 		return false;
// 	}
	
// 	return [instance isKindOfClass:classWithScriptView->class];
// }


// @implementation EJClassLoader

// + (JSClassRef)getJSClass:(id)class {
// 	NSAssert(EJGlobalJSClassCache != nil, @"Attempt to access class cache without a loader present.");
	
// 	// Try the cache first
// 	JSClassRef jsClass = [EJGlobalJSClassCache[class] pointerValue];
// 	if( jsClass ) {
// 		return jsClass;
// 	}
	
// 	// Still here? Create and insert into cache
// 	jsClass = [self createJSClass:class];
// 	EJGlobalJSClassCache[class] = [NSValue valueWithPointer:jsClass];
// 	return jsClass;
// }

// + (JSClassRef)createJSClass:(id)class {
// 	// Gather all class methods that return C callbacks for this class or it's parents
// 	NSMutableArray *methods = [[NSMutableArray alloc] init];
// 	NSMutableArray *properties = [[NSMutableArray alloc] init];
		
// 	// Traverse this class and all its super classes
// 	Class base = EJBindingBase.class;
// 	for( Class sc = class; sc != base && [sc isSubclassOfClass:base]; sc = sc.superclass ) {
	
// 		// Traverse all class methods for this class; i.e. all classes that are defined with the
// 		// EJ_BIND_FUNCTION, EJ_BIND_GET or EJ_BIND_SET macros
// 		u_int count;
// 		Method *methodList = class_copyMethodList(object_getClass(sc), &count);
// 		for (int i = 0; i < count ; i++) {
// 			SEL selector = method_getName(methodList[i]);
// 			NSString *name = NSStringFromSelector(selector);
			
// 			if( [name hasPrefix:@"_ptr_to_func_"] ) {
// 				[methods addObject: [name substringFromIndex:sizeof("_ptr_to_func_")-1] ];
// 			}
// 			else if( [name hasPrefix:@"_ptr_to_get_"] ) {
// 				// We only look for getters - a property that has a setter, but no getter will be ignored
// 				[properties addObject: [name substringFromIndex:sizeof("_ptr_to_get_")-1] ];
// 			}
// 		}
// 		free(methodList);
// 	}
	
	
// 	// Set up the JSStaticValue struct array
// 	JSStaticValue *values = calloc( properties.count + 1, sizeof(JSStaticValue) );
// 	for( int i = 0; i < properties.count; i++ ) {
// 		NSString *name = properties[i];
		
// 		values[i].name = name.UTF8String;
// 		values[i].attributes = kJSPropertyAttributeDontDelete;
		
// 		SEL get = NSSelectorFromString([@"_ptr_to_get_" stringByAppendingString:name]);
// 		values[i].getProperty = (JSObjectGetPropertyCallback)[class performSelector:get];
		
// 		// Property has a setter? Otherwise mark as read only
// 		SEL set = NSSelectorFromString([@"_ptr_to_set_"stringByAppendingString:name]);
// 		if( [class respondsToSelector:set] ) {
// 			values[i].setProperty = (JSObjectSetPropertyCallback)[class performSelector:set];
// 		}
// 		else {
// 			values[i].attributes |= kJSPropertyAttributeReadOnly;
// 		}
// 	}
	
// 	// Set up the JSStaticFunction struct array
// 	JSStaticFunction *functions = calloc( methods.count + 1, sizeof(JSStaticFunction) );
// 	for( int i = 0; i < methods.count; i++ ) {
// 		NSString *name = methods[i];
				
// 		functions[i].name = name.UTF8String;
// 		functions[i].attributes = kJSPropertyAttributeDontDelete;
		
// 		SEL call = NSSelectorFromString([@"_ptr_to_func_" stringByAppendingString:name]);
// 		functions[i].callAsFunction = (JSObjectCallAsFunctionCallback)[class performSelector:call];
// 	}
	
// 	JSClassDefinition classDef = kJSClassDefinitionEmpty;
// 	classDef.className = class_getName(class) + sizeof(EJ_BINDING_CLASS_PREFIX)-1;
// 	classDef.finalize = EJBindingBaseFinalize;
// 	classDef.staticValues = values;
// 	classDef.staticFunctions = functions;
// 	JSClassRef jsClass = JSClassCreate(&classDef);
	
// 	free( values );
// 	free( functions );
	
// 	[properties release];
// 	[methods release];
	
// 	return jsClass;
// }

// - (id)initWithScriptView:(EJJavaScriptView *)scriptView name:(NSString *)name {
// 	if( self = [super init] ) {
// 		JSGlobalContextRef context = scriptView.jsGlobalContext;
		
// 		// Create or retain the global constructor class
// 		if( !EJGlobalConstructorClass ) {
// 			JSClassDefinition constructorClassDef = kJSClassDefinitionEmpty;
// 			constructorClassDef.callAsConstructor = EJCallAsConstructor;
// 			constructorClassDef.hasInstance = EJConstructorHasInstance;
// 			constructorClassDef.finalize = EJConstructorFinalize;
// 			EJGlobalConstructorClass = JSClassCreate(&constructorClassDef);
// 		}
// 		else {
// 			JSClassRetain(EJGlobalConstructorClass);
// 		}
		
// 		// Create the collection class and attach it to the global context with
// 		// the given name
// 		JSClassDefinition loaderClassDef = kJSClassDefinitionEmpty;
// 		loaderClassDef.getProperty = EJGetNativeClass;
// 		JSClassRef loaderClass = JSClassCreate(&loaderClassDef);
		
// 		JSObjectRef global = JSContextGetGlobalObject(context);
		
// 		JSObjectRef loader = JSObjectMake(context, loaderClass, scriptView);
// 		JSStringRef jsName = JSStringCreateWithUTF8CString(name.UTF8String);
// 		JSObjectSetProperty(
// 			context, global, jsName, loader,
// 			(kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly),
// 			NULL
// 		);
// 		JSStringRelease(jsName);
		
		
// 		// Create or retain the global Class cache
// 		if( !EJGlobalJSClassCache ) {
// 			EJGlobalJSClassCache = [[NSMutableDictionary alloc] initWithCapacity:16];
// 		}
// 		else {
// 			[EJGlobalJSClassCache retain];
// 		}
// 	}
// 	return self;
// }

// - (void)dealloc {
// 	// If we are the last Collection to hold on to the Class cache, release it and
// 	// set it to nil, so it can be properly re-created if needed.
// 	if( EJGlobalJSClassCache.retainCount == 1 ) {
// 		[EJGlobalJSClassCache release];
// 		EJGlobalJSClassCache = nil;
// 	}
// 	else {
// 		[EJGlobalJSClassCache release];
// 	}
	
// 	JSClassRelease(EJGlobalConstructorClass);
// 	[super dealloc];
// }


// @end

