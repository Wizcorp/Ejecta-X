#ifndef __EJ_JAVASCRIPT_VIEW_H__
#define __EJ_JAVASCRIPT_VIEW_H__

#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <jni.h>
#include <android/log.h>
#include <GLES/gl.h>
#endif

#include <string>
#include <set>
#include <JavaScriptCore/JavaScriptCore.h>

#include "EJConvert.h"
#include "EJCanvas/EJCanvasContext.h"
//#include "EJPresentable.h"

#include "EJAudio/EJSharedOpenALManager.h"
#include "EJCanvas/EJSharedTextureCache.h"
#include "EJSharedOpenGLContext.h"
#include "EJNonRetainingProxy.h"

#include "EJCocoa/support/nsMacros.h"
#include "EJCocoa/NSDictionary.h"
#include "EJCocoa/NSObject.h"
#include "EJCocoa/NSString.h"
#include "EJCocoa/NSSet.h"
#include "EJCocoa/NSValue.h"

using namespace std;

#define EJECTA_VERSION "1.2"
#define EJECTA_DEFAULT_APP_FOLDER "App/"

#define EJECTA_BOOT_JS "Ejecta.js"

class EJTouchDelegate {
public:
	virtual void triggerEvent(NSString * name, NSSet * all, NSSet * changed, NSSet * remaining)=0;
};

class EJDeviceMotionDelegate {
public:
	virtual void triggerDeviceMotionEvents()=0;
};

class EJLifecycleDelegate {
public:
	virtual void resume()=0;
	virtual void pause()=0;
};

class EJTimerCollection;
class EJClassLoader;

class EJJavaScriptView : NSObject {
	
	EJNonRetainingProxy	*proxy;
	EJClassLoader *classLoader;

	EJTimerCollection *timers;
	EJSharedTextureCache *textureCache;
	EJSharedOpenALManager *openALManager;
	
	//EAGLContext *glCurrentContext;
	
	//CADisplayLink *displayLink;

	//NSOperationQueue *backgroundQueue;

	char* mainBundle;
	
public:
	NSString *appFolder;
	
	bool pauseOnEnterBackground;
	bool hasScreenCanvas;

	bool isPaused;

	JSGlobalContextRef jsGlobalContext;
	int height, width;
	
	EJSharedOpenGLContext *openGLContext;

	NSObjectT<EJLifecycleDelegate> *lifecycleDelegate;
	NSObjectT<EJTouchDelegate> *touchDelegate;
	NSObjectT<EJDeviceMotionDelegate> *deviceMotionDelegate;

	EJCanvasContext *currentRenderingContext;
	EJCanvasContext *screenRenderingContext;

	// Public for fast access in bound functions
	JSValueRef jsUndefined;

	EJJavaScriptView();
    EJJavaScriptView(int w, int h);
    EJJavaScriptView(int w, int h, const char* folder);
    virtual ~EJJavaScriptView(void);

    void setMainBundle(const char* path);
    void setScreenSize(int w, int h);
    void init(int w, int h, const char* folder);

    void setPauseOnEnterBackground(bool pauses);
    void removeObserverForKeyPaths();
    void observeKeyPaths();

	NSString * pathForResource(NSString * resourcePath);
	void loadScriptAtPath(NSString * path);
	void loadScript(NSString * script, NSString * sourceURL);

	void run(void);
	void pause(void);
	void resume(void);
	void clearCaches(void);

	void setCurrentRenderingContext(EJCanvasContext * renderingContext);

	JSValueRef loadModuleWithId(NSString * moduleId, JSValueRef module, JSValueRef exports);
 	JSValueRef invokeCallback(JSObjectRef callback, JSObjectRef thisObject, size_t argc, const JSValueRef argv[]);
	void logException(JSValueRef exception, JSContextRef ctxp);

	void touchesBegan(NSSet * touches, int * event);
	void touchesEnded(NSSet * touches, int * event);
	void touchesCancelled(NSSet * touches, int * event);
	void touchesMoved(NSSet * touches, int * event);

	JSValueRef createTimer(JSContextRef ctx, size_t argc, const JSValueRef argv[], bool repeat);
	JSValueRef deleteTimer(JSContextRef ctx, size_t argc, const JSValueRef argv[]);

};

#endif // __EJ_JAVASCRIPT_VIEW_H__

// #import <UIKit/UIKit.h>
// #import <QuartzCore/QuartzCore.h>
// #import <JavaScriptCore/JavaScriptCore.h>
// #import "EJConvert.h"
// #import "EJCanvasContext.h"
// #import "EJPresentable.h"

// #import "EJSharedOpenALManager.h"
// #import "EJSharedTextureCache.h"
// #import "EJSharedOpenGLContext.h"
// #import "EJNonRetainingProxy.h"

// #define EJECTA_VERSION @"1.2"
// #define EJECTA_DEFAULT_APP_FOLDER @"App/"

// #define EJECTA_BOOT_JS @"../Ejecta.js"


// @protocol EJTouchDelegate
// - (void)triggerEvent:(NSString *)name all:(NSSet *)all changed:(NSSet *)changed remaining:(NSSet *)remaining;
// @end

// @protocol EJDeviceMotionDelegate
// - (void)triggerDeviceMotionEvents;
// @end

// @protocol EJLifecycleDelegate
// - (void)resume;
// - (void)pause;
// @end

// @class EJTimerCollection;
// @class EJClassLoader;

// @interface EJJavaScriptView : UIView {
// 	NSString *appFolder;
	
// 	BOOL pauseOnEnterBackground;
// 	BOOL hasScreenCanvas;

// 	BOOL isPaused;
	
// 	EJNonRetainingProxy	*proxy;

// 	JSGlobalContextRef jsGlobalContext;
// 	EJClassLoader *classLoader;

// 	EJTimerCollection *timers;
	
// 	EJSharedOpenGLContext *openGLContext;
// 	EJSharedTextureCache *textureCache;
// 	EJSharedOpenALManager *openALManager;
	
// 	EJCanvasContext *currentRenderingContext;
// 	EAGLContext *glCurrentContext;
	
// 	CADisplayLink *displayLink;

// 	NSObject<EJLifecycleDelegate> *lifecycleDelegate;
// 	NSObject<EJTouchDelegate> *touchDelegate;
// 	NSObject<EJDeviceMotionDelegate> *deviceMotionDelegate;
// 	EJCanvasContext<EJPresentable> *screenRenderingContext;

// 	NSOperationQueue *backgroundQueue;
	
// 	// Public for fast access in bound functions
// 	@public JSValueRef jsUndefined;
// }

// @property (nonatomic, copy) NSString *appFolder;

// @property (nonatomic, assign) BOOL pauseOnEnterBackground;
// @property (nonatomic, assign, getter = isPaused) BOOL isPaused; // Pauses drawing/updating of the JSView
// @property (nonatomic, assign) BOOL hasScreenCanvas;

// @property (nonatomic, readonly) JSGlobalContextRef jsGlobalContext;
// @property (nonatomic, readonly) EJSharedOpenGLContext *openGLContext;

// @property (nonatomic, retain) NSObject<EJLifecycleDelegate> *lifecycleDelegate;
// @property (nonatomic, retain) NSObject<EJTouchDelegate> *touchDelegate;
// @property (nonatomic, retain) NSObject<EJDeviceMotionDelegate> *deviceMotionDelegate;

// @property (nonatomic, retain) EJCanvasContext *currentRenderingContext;
// @property (nonatomic, retain) EJCanvasContext<EJPresentable> *screenRenderingContext;

// @property (nonatomic, retain) NSOperationQueue *backgroundQueue;

// - (id)initWithFrame:(CGRect)frame appFolder:(NSString *)folder;

// - (void)loadScriptAtPath:(NSString *)path;
// - (void)loadScript:(NSString *)script sourceURL:(NSString *)sourceURL;

// - (void)clearCaches;

// - (JSValueRef)invokeCallback:(JSObjectRef)callback thisObject:(JSObjectRef)thisObject argc:(size_t)argc argv:(const JSValueRef [])argv;
// - (NSString *)pathForResource:(NSString *)resourcePath;
// - (JSValueRef)deleteTimer:(JSContextRef)ctx argc:(size_t)argc argv:(const JSValueRef [])argv;
// - (JSValueRef)loadModuleWithId:(NSString *)moduleId module:(JSValueRef)module exports:(JSValueRef)exports;
// - (JSValueRef)createTimer:(JSContextRef)ctxp argc:(size_t)argc argv:(const JSValueRef [])argv repeat:(BOOL)repeat;

// @end
