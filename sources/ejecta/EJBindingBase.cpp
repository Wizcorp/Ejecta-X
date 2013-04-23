#include "EJBindingBase.h"
#include "EJClassLoader.h"

EJBindingBase::EJBindingBase() : jsObject(0)
{

}

EJBindingBase::EJBindingBase(JSContextRef ctxp, size_t argc, const JSValueRef argv[])
{

}

EJBindingBase::~EJBindingBase()
{

}

void EJBindingBase::init(JSContextRef ctxp,  size_t argc, const JSValueRef argv[])
{

}

void EJBindingBase::create(JSObjectRef obj, EJJavaScriptView * view)
{
	jsObject = obj;
	scriptView = view;
}

void EJBindingBase::prepareGarbageCollection()
{
	// Called in EJBindingBaseFinalize before sending 'release'.
	// Cancel loading callbacks and the like here.
}

JSObjectRef EJBindingBase::createJSObject(JSContextRef ctx, EJJavaScriptView * scriptViewp, EJBindingBase * instance)
{
	// Create JSObject with the JSClass for this ObjC-Class
	EJBindingBase* tmp = new EJBindingBase();
	tmp->autorelease();
	JSObjectRef obj = JSObjectMake( ctx, EJClassLoader::getJSClass(tmp), NULL );
	
	// The JSObject retains the instance; it will be released by EJBindingBaseFinalize
	instance->retain();
	JSObjectSetPrivate( obj, (void *)instance );
	instance->create(obj, scriptViewp);
	
	return obj;
}

void EJBindingBaseFinalize(JSObjectRef object) {
	EJBindingBase* instance = (EJBindingBase*) JSObjectGetPrivate(object);
	instance->prepareGarbageCollection();
	instance->release();
}


// #import "EJBindingBase.h"
// #import "EJClassLoader.h"
// #import <objc/runtime.h>


// @implementation EJBindingBase
// @synthesize scriptView;

// - (id)initWithContext:(JSContextRef)ctxp argc:(size_t)argc argv:(const JSValueRef [])argv {
// 	if( self = [super init] ) {
// 	}
// 	return self;
// }

// - (void)createWithJSObject:(JSObjectRef)obj scriptView:(EJJavaScriptView *)view {
// 	jsObject = obj;
// 	scriptView = view;
// }

// - (void)prepareGarbageCollection {
// 	// Called in EJBindingBaseFinalize before sending 'release'.
// 	// Cancel loading callbacks and the like here.
// }

// + (JSObjectRef)createJSObjectWithContext:(JSContextRef)ctx
// 	scriptView:(EJJavaScriptView *)scriptViewp
// 	instance:(EJBindingBase *)instance
// {
// 	// Create JSObject with the JSClass for this ObjC-Class
// 	JSObjectRef obj = JSObjectMake( ctx, [EJClassLoader getJSClass:self], NULL );
	
// 	// The JSObject retains the instance; it will be released by EJBindingBaseFinalize
// 	JSObjectSetPrivate( obj, (void *)[instance retain] );
// 	[instance createWithJSObject:obj scriptView:scriptViewp];
	
// 	return obj;
// }

// void EJBindingBaseFinalize(JSObjectRef object) {
// 	EJBindingBase *instance = (EJBindingBase *)JSObjectGetPrivate(object);
// 	[instance prepareGarbageCollection];
// 	[instance release];
// }


// @end
