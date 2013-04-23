#include "EJJavaScriptView.h"
#include "EJTimer.h"
#include "EJBindingBase.h"
#include "EJClassLoader.h"


#pragma mark -
#pragma mark Ejecta view Implementation



EJJavaScriptView::EJJavaScriptView() : currentRenderingContext(0), screenRenderingContext(0)
{
	init(0, 0, EJECTA_DEFAULT_APP_FOLDER);
}

EJJavaScriptView::EJJavaScriptView(int w, int h) : currentRenderingContext(0), screenRenderingContext(0)
{
	init(w, h, EJECTA_DEFAULT_APP_FOLDER);
}

EJJavaScriptView::EJJavaScriptView(int w, int h, const char* folder) : currentRenderingContext(0), screenRenderingContext(0)
{
	init(w, h, folder);
}

void EJJavaScriptView::setMainBundle(const char* path)
{
	if(mainBundle)free(mainBundle);
    int len = (strlen(path) + 1);
    mainBundle = (char *)malloc(len * sizeof(char));
    memset(mainBundle, 0, len);
#ifdef _WINDOWS
	_snprintf(mainBundle, len, "%s", path);
#else
	snprintf(mainBundle, len, "%s", path);
#endif
}

void EJJavaScriptView::setScreenSize(int w, int h)
{
	height = h;
	width = w;
	if(screenRenderingContext) {
		screenRenderingContext->setScreenSize(w, h);
		setCurrentRenderingContext((EJCanvasContext *)screenRenderingContext);
	}
}

void EJJavaScriptView::init(int w, int h, const char* folder){
	appFolder = NSStringMake(folder);
	appFolder->retain();

	isPaused = false;

	// CADisplayLink (and NSNotificationCenter?) retains it's target, but this
	// is causing a retain loop - we can't completely release the scriptView
	// from the outside.
	// So we're using a "weak proxy" that doesn't retain the scriptView; we can
	// then just invalidate the CADisplayLink in our dealloc and be done with it.
	proxy = new EJNonRetainingProxy();
	proxy->retain();
	
	this->pauseOnEnterBackground = true;
	
	// Limit all background operations (image & sound loading) to one thread
	//backgroundQueue = [[NSOperationQueue alloc] init];
	//backgroundQueue.maxConcurrentOperationCount = 1;
	
	timers = new EJTimerCollection(this);
	
	//displayLink = [[CADisplayLink displayLinkWithTarget:proxy selector:@selector(run:)] retain];
	//[displayLink setFrameInterval:1];
	//[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	
	// Create the global JS context in its own group, so it can be released properly
	jsGlobalContext = JSGlobalContextCreateInGroup(NULL, NULL);
	jsUndefined = JSValueMakeUndefined(jsGlobalContext);
	JSValueProtect(jsGlobalContext, jsUndefined);
	
	// Attach all native class constructors to 'Ejecta'
	classLoader = new EJClassLoader(this, NSStringMake("Ejecta"));
	
	
	// Retain the caches here, so even if they're currently unused in JavaScript,
	// they will persist until the last scriptView is released
	textureCache = new EJSharedTextureCache();textureCache->retain();
	openALManager = new EJSharedOpenALManager();openALManager->retain();
	openGLContext = new EJSharedOpenGLContext();openGLContext->retain();
	
	// Create the OpenGL context for Canvas2D
	//glCurrentContext = openGLContext.glContext2D;
	//[EAGLContext setCurrentContext:glCurrentContext];

	loadScriptAtPath(NSStringMake(EJECTA_BOOT_JS));
}

EJJavaScriptView::~EJJavaScriptView()
{
	// Wait until all background operations are finished. If we would just release the
	// backgroundQueue it would cancel running operations (such as texture loading) and
	// could keep some dependencies dangling
	//[backgroundQueue waitUntilAllOperationsAreFinished];
	//[backgroundQueue release];
	
	// Careful, order is important! The JS context has to be released first; it will release
	// the canvas objects which still need the openGLContext to be present, to release
	// textures etc.
	// Set 'jsGlobalContext' to null before releasing it, because it may be referenced by
	// bound objects' dealloc method
	pause();
	JSValueUnprotect(jsGlobalContext, jsUndefined);
	JSGlobalContextRef ctxref = jsGlobalContext;
	jsGlobalContext = NULL;
	JSGlobalContextRelease(ctxref);
	
	// Remove from notification center
	this->pauseOnEnterBackground = false;
	
	// Remove from display link
	//[displayLink invalidate];
	//[displayLink release];
	
	textureCache->release();
	openALManager->release();
	classLoader->release();
	
	screenRenderingContext->finish();
	screenRenderingContext->release();
	currentRenderingContext->release();
	
	touchDelegate->release();
	lifecycleDelegate->release();
	deviceMotionDelegate->release();
	
	timers->release();
	
	openGLContext->release();
	appFolder->release();

	if(mainBundle)free(mainBundle);
}

void EJJavaScriptView::setPauseOnEnterBackground(bool pauses)
{
	// NSArray *pauseN = @[
	// 	UIApplicationWillResignActiveNotification,
	// 	UIApplicationDidEnterBackgroundNotification,
	// 	UIApplicationWillTerminateNotification
	// ];
	// NSArray *resumeN = @[
	// 	UIApplicationWillEnterForegroundNotification,
	// 	UIApplicationDidBecomeActiveNotification
	// ];
	
	// if (pauses) {
	// 	[self observeKeyPaths:pauseN selector:@selector(pause)];
	// 	[self observeKeyPaths:resumeN selector:@selector(resume)];
	// } 
	// else {
	// 	[self removeObserverForKeyPaths:pauseN];
	// 	[self removeObserverForKeyPaths:resumeN];
	// }
	pauseOnEnterBackground = pauses;
}

void EJJavaScriptView::removeObserverForKeyPaths()
{
	// NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	// for( NSString *name in keyPaths ) {
	// 	[nc removeObserver:proxy name:name object:nil];
	// }
}

void EJJavaScriptView::observeKeyPaths()
{
	// NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
	// for( NSString *name in keyPaths ) {
	// 	[nc addObserver:proxy selector:selector name:name object:nil];
	// }
}


#pragma mark -
#pragma mark Script loading and execution

NSString * EJJavaScriptView::pathForResource(NSString * resourcePath)
{
 	string full_path = string(mainBundle) + string("/") + string(appFolder->getCString()) + resourcePath->getCString();
	return NSStringMake(full_path);
}

void EJJavaScriptView::loadScriptAtPath(NSString * path)
{
	NSString * script = NSString::createWithContentsOfFile(pathForResource(path)->getCString());
	loadScript(script, path);
}

void EJJavaScriptView::loadScript(NSString * script, NSString * sourceURL)
{
	
	if( !script || script->length() == 0 ) {
		NSLOG("Error: No or empty script given : %s", sourceURL->getCString() );
		return;
	}

	JSStringRef scriptJS = JSStringCreateWithUTF8CString(script->getCString());
	JSStringRef sourceURLJS = NULL;
    
	if( sourceURL->length() > 0 ) {
		sourceURLJS = JSStringCreateWithUTF8CString(sourceURL->getCString());
	}
	
	JSValueRef exception = NULL;
	JSEvaluateScript( jsGlobalContext, scriptJS, NULL, sourceURLJS, 0, &exception );
	logException(exception, jsGlobalContext);

	JSStringRelease( scriptJS );

	if ( sourceURLJS ) {
		JSStringRelease( sourceURLJS );
	}
}

JSValueRef EJJavaScriptView::loadModule(NSString * moduleId, JSValueRef module, JSValueRef exports)
{
	NSString * path = NSStringMake(moduleId->getCString() + string(".js"));
	NSString * script = NSString::createWithContentsOfFile(pathForResource(path)->getCString());
	
	if( !script ) {
		NSLOG("Error: Can't Find Module %s", moduleId->getCString() );
		return NULL;
	}
	
	NSLOG("Loading Module: %s", moduleId->getCString() );
	
	JSStringRef scriptJS = JSStringCreateWithUTF8CString(script->getCString());
	JSStringRef pathJS = JSStringCreateWithUTF8CString(path->getCString());
	JSStringRef parameterNames[] = {
		JSStringCreateWithUTF8CString("module"),
		JSStringCreateWithUTF8CString("exports"),
	};
	
	JSValueRef exception = NULL;
	JSObjectRef func = JSObjectMakeFunction( jsGlobalContext, NULL, 2,  parameterNames, scriptJS, pathJS, 0, &exception );
	
	JSStringRelease( scriptJS );
	JSStringRelease( pathJS );
	JSStringRelease(parameterNames[0]);
	JSStringRelease(parameterNames[1]);
	
	if( exception ) {
		logException(exception, jsGlobalContext);
		return NULL;
	}
	
	JSValueRef params[] = { module, exports };
	return invokeCallback(func, NULL, 2, params);
}

JSValueRef EJJavaScriptView::invokeCallback(JSObjectRef callback, JSObjectRef thisObject, size_t argc, const JSValueRef argv[])
{
	if( !jsGlobalContext ) { return NULL; } // May already have been released

	JSValueRef exception = NULL;
	JSValueRef result = JSObjectCallAsFunction( jsGlobalContext, callback, thisObject, argc, argv, &exception );
	logException(exception,jsGlobalContext);
	return result;
}

void EJJavaScriptView::logException(JSValueRef valueAsexception, JSContextRef ctxp)
{
	if( !valueAsexception ) return;
	
	JSStringRef jsLinePropertyName = JSStringCreateWithUTF8CString("line");
	JSStringRef jsFilePropertyName = JSStringCreateWithUTF8CString("sourceURL");
	
	JSObjectRef exObject = JSValueToObject( ctxp, valueAsexception, NULL );
	JSValueRef valueAsline = JSObjectGetProperty( ctxp, exObject, jsLinePropertyName, NULL );
	JSValueRef valueAsfile = JSObjectGetProperty( ctxp, exObject, jsFilePropertyName, NULL );
	

    JSStringRef jsexception = JSValueToStringCopy(ctxp, valueAsexception, NULL);
    JSStringRef jsline = JSValueToStringCopy(ctxp, valueAsline, NULL);
    JSStringRef jsfile = JSValueToStringCopy(ctxp, valueAsfile, NULL);

    size_t jsexceptionSize = JSStringGetMaximumUTF8CStringSize(jsexception);
    char* exception = (char*)malloc(jsexceptionSize);
    JSStringGetUTF8CString(jsexception, exception, jsexceptionSize);

    size_t jslineSize = JSStringGetMaximumUTF8CStringSize(jsline);
    char* line = (char*)malloc(jslineSize);
    JSStringGetUTF8CString(jsline, line, jslineSize);

    size_t jsfileSize = JSStringGetMaximumUTF8CStringSize(jsfile);
    char* file = (char*)malloc(jsfileSize);
    JSStringGetUTF8CString(jsfile, file, jsfileSize);

	NSLOG("%s at line %s in %s", exception, line, file);
	

    free(exception);
    free(line);
    free(file);
    JSStringRelease(jsexception);
    JSStringRelease(jsline);
    JSStringRelease(jsfile);

	JSStringRelease( jsLinePropertyName );
	JSStringRelease( jsFilePropertyName );
}


#pragma mark -
#pragma mark Run loop

void EJJavaScriptView::run(void)
{

	if( isPaused ) { return; }
	
	// We rather poll for device motion updates at the beginning of each frame instead of
	// spamming out updates that will never be seen.
	if(deviceMotionDelegate) deviceMotionDelegate->triggerDeviceMotionEvents();

	// Check all timers
	if(timers)timers->update();
	
	if (screenRenderingContext)
	{
		// Redraw the canvas
		setCurrentRenderingContext((EJCanvasContext *)screenRenderingContext);
		screenRenderingContext->present();
		
	}
}

void EJJavaScriptView::pause(void)
{
	if( isPaused ) { return; }

	lifecycleDelegate->pause();
	// [displayLink removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	screenRenderingContext->finish();
	isPaused = true;
}

void EJJavaScriptView::resume(void)
{
	if( !isPaused ) { return; }
	
	lifecycleDelegate->resume();
	// [EAGLContext setCurrentContext:glCurrentContext];
	// [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	isPaused = false;
}

void EJJavaScriptView::clearCaches(void)
{
	JSGarbageCollect(jsGlobalContext);
}

void EJJavaScriptView::setCurrentRenderingContext(EJCanvasContext * renderingContext)
{
	if( renderingContext != currentRenderingContext ) {
		if(currentRenderingContext) {
			currentRenderingContext->flushBuffers();
			currentRenderingContext->release();
		}
		
		// Switch GL Context if different
		// if( renderingContext && renderingContext.glContext != glCurrentContext ) {
		// 	glFlush();
		// 	glCurrentContext = renderingContext.glContext;
		// 	[EAGLContext setCurrentContext:glCurrentContext];
		// }
		
		if(renderingContext){
			renderingContext->prepare();
			renderingContext->retain();
		}
		currentRenderingContext = renderingContext;
	}
}

#pragma mark -
#pragma mark Touch handlers

void EJJavaScriptView::touchesBegan(NSSet * touches, int * event)
{
	//[touchDelegate triggerEvent:@"touchstart" all:event.allTouches changed:touches remaining:event.allTouches];
}

void EJJavaScriptView::touchesEnded(NSSet * touches, int * event)
{
	// NSMutableSet *remaining = [event.allTouches mutableCopy];
	// [remaining minusSet:touches];
	
	// [touchDelegate triggerEvent:@"touchend" all:event.allTouches changed:touches remaining:remaining];
	// [remaining release];
}

void EJJavaScriptView::touchesCancelled(NSSet * touches, int * event)
{
	touchesEnded(touches, event);
}

void EJJavaScriptView::touchesMoved(NSSet * touches, int * event)
{
	//[touchDelegate triggerEvent:@"touchmove" all:event.allTouches changed:touches remaining:event.allTouches];
}

//TODO: Does this belong in this class?
#pragma mark
#pragma mark Timers

JSValueRef EJJavaScriptView::createTimer(JSContextRef ctxp, size_t argc, const JSValueRef argv[],  bool repeat)
{
	if( argc != 2 || !JSValueIsObject(ctxp, argv[0]) || !JSValueIsNumber(jsGlobalContext, argv[1]) ) {
		return NULL;
	}
	
	JSObjectRef func = JSValueToObject(ctxp, argv[0], NULL);
	float interval = (float)JSValueToNumber(ctxp, argv[1], NULL) * 1000.0f;
	
	// Make sure short intervals (< 18ms) run each frame
	if( interval < 0.018 ) {
		interval = 0;
	}
	
	int timerId = timers->scheduleCallback(func, interval, repeat);
	return JSValueMakeNumber( ctxp, timerId );
}

JSValueRef EJJavaScriptView::deleteTimer(JSContextRef ctxp, size_t argc, const JSValueRef argv[])
{
	if( argc != 1 || !JSValueIsNumber(ctxp, argv[0]) ) return NULL;
	
	timers->cancelId((int)JSValueToNumber(ctxp, argv[0], NULL));
	return NULL;
}

// #import "EJJavaScriptView.h"
// #import "EJTimer.h"
// #import "EJBindingBase.h"
// #import "EJClassLoader.h"
// #import <objc/runtime.h>


// #pragma mark -
// #pragma mark Ejecta view Implementation

// @implementation EJJavaScriptView

// @synthesize appFolder;

// @synthesize pauseOnEnterBackground;
// @synthesize isPaused;
// @synthesize hasScreenCanvas;
// @synthesize jsGlobalContext;

// @synthesize currentRenderingContext;
// @synthesize openGLContext;

// @synthesize lifecycleDelegate;
// @synthesize touchDelegate;
// @synthesize deviceMotionDelegate;
// @synthesize screenRenderingContext;

// @synthesize backgroundQueue;

// - (id)initWithFrame:(CGRect)frame {
// 	return [self initWithFrame:frame appFolder:EJECTA_DEFAULT_APP_FOLDER];
// }

// - (id)initWithFrame:(CGRect)frame appFolder:(NSString *)folder {
// 	if( self = [super initWithFrame:frame] ) {
// 		appFolder = [folder retain];
		
// 		isPaused = false;

// 		// CADisplayLink (and NSNotificationCenter?) retains it's target, but this
// 		// is causing a retain loop - we can't completely release the scriptView
// 		// from the outside.
// 		// So we're using a "weak proxy" that doesn't retain the scriptView; we can
// 		// then just invalidate the CADisplayLink in our dealloc and be done with it.
// 		proxy = [[EJNonRetainingProxy proxyWithTarget:self] retain];
		
// 		self.pauseOnEnterBackground = YES;
		
// 		// Limit all background operations (image & sound loading) to one thread
// 		backgroundQueue = [[NSOperationQueue alloc] init];
// 		backgroundQueue.maxConcurrentOperationCount = 1;
		
// 		timers = [[EJTimerCollection alloc] initWithScriptView:self];
		
// 		displayLink = [[CADisplayLink displayLinkWithTarget:proxy selector:@selector(run:)] retain];
// 		[displayLink setFrameInterval:1];
// 		[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		
// 		// Create the global JS context in its own group, so it can be released properly
// 		jsGlobalContext = JSGlobalContextCreateInGroup(NULL, NULL);
// 		jsUndefined = JSValueMakeUndefined(jsGlobalContext);
// 		JSValueProtect(jsGlobalContext, jsUndefined);
		
// 		// Attach all native class constructors to 'Ejecta'
// 		classLoader = [[EJClassLoader alloc] initWithScriptView:self name:@"Ejecta"];
		
		
// 		// Retain the caches here, so even if they're currently unused in JavaScript,
// 		// they will persist until the last scriptView is released
// 		textureCache = [[EJSharedTextureCache instance] retain];
// 		openALManager = [[EJSharedOpenALManager instance] retain];
// 		openGLContext = [[EJSharedOpenGLContext instance] retain];
		
// 		// Create the OpenGL context for Canvas2D
// 		glCurrentContext = openGLContext.glContext2D;
// 		[EAGLContext setCurrentContext:glCurrentContext];
		
// 		[self loadScriptAtPath:EJECTA_BOOT_JS];
// 	}
// 	return self;
// }

// - (void)dealloc {
// 	// Wait until all background operations are finished. If we would just release the
// 	// backgroundQueue it would cancel running operations (such as texture loading) and
// 	// could keep some dependencies dangling
// 	[backgroundQueue waitUntilAllOperationsAreFinished];
// 	[backgroundQueue release];
	
// 	// Careful, order is important! The JS context has to be released first; it will release
// 	// the canvas objects which still need the openGLContext to be present, to release
// 	// textures etc.
// 	// Set 'jsGlobalContext' to null before releasing it, because it may be referenced by
// 	// bound objects' dealloc method
// 	JSValueUnprotect(jsGlobalContext, jsUndefined);
// 	JSGlobalContextRef ctxref = jsGlobalContext;
// 	jsGlobalContext = NULL;
// 	JSGlobalContextRelease(ctxref);
	
// 	// Remove from notification center
// 	self.pauseOnEnterBackground = false;
	
// 	// Remove from display link
// 	[displayLink invalidate];
// 	[displayLink release];
	
// 	[textureCache release];
// 	[openALManager release];
// 	[classLoader release];
	
// 	[screenRenderingContext finish];
// 	[screenRenderingContext release];
// 	[currentRenderingContext release];
	
// 	[touchDelegate release];
// 	[lifecycleDelegate release];
// 	[deviceMotionDelegate release];
	
// 	[timers release];
	
// 	[openGLContext release];
// 	[appFolder release];
// 	[super dealloc];
// }

// - (void)setPauseOnEnterBackground:(BOOL)pauses {
// 	NSArray *pauseN = @[
// 		UIApplicationWillResignActiveNotification,
// 		UIApplicationDidEnterBackgroundNotification,
// 		UIApplicationWillTerminateNotification
// 	];
// 	NSArray *resumeN = @[
// 		UIApplicationWillEnterForegroundNotification,
// 		UIApplicationDidBecomeActiveNotification
// 	];
	
// 	if (pauses) {
// 		[self observeKeyPaths:pauseN selector:@selector(pause)];
// 		[self observeKeyPaths:resumeN selector:@selector(resume)];
// 	} 
// 	else {
// 		[self removeObserverForKeyPaths:pauseN];
// 		[self removeObserverForKeyPaths:resumeN];
// 	}
// 	pauseOnEnterBackground = pauses;
// }

// - (void)removeObserverForKeyPaths:(NSArray*)keyPaths {
// 	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
// 	for( NSString *name in keyPaths ) {
// 		[nc removeObserver:proxy name:name object:nil];
// 	}
// }

// - (void)observeKeyPaths:(NSArray*)keyPaths selector:(SEL)selector {
// 	NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
// 	for( NSString *name in keyPaths ) {
// 		[nc addObserver:proxy selector:selector name:name object:nil];
// 	}
// }


// #pragma mark -
// #pragma mark Script loading and execution

// - (NSString *)pathForResource:(NSString *)path {
// 	return [NSString stringWithFormat:@"%@/%@%@", [[NSBundle mainBundle] resourcePath], appFolder, path];
// }

// - (void)loadScriptAtPath:(NSString *)path {
// 	NSString *script = [NSString stringWithContentsOfFile:[self pathForResource:path]
// 		encoding:NSUTF8StringEncoding error:NULL];
	
// 	[self loadScript:script sourceURL:path];
// }

// - (void)loadScript:(NSString *)script sourceURL:(NSString *)sourceURL {
// 	if( !script || script.length == 0 ) {
// 		NSLog(@"Error: No or empty script given");
// 		return;
// 	}
    
// 	JSStringRef scriptJS = JSStringCreateWithCFString((CFStringRef)script);
// 	JSStringRef sourceURLJS = NULL;
    
// 	if( [sourceURL length] > 0 ) {
// 		sourceURLJS = JSStringCreateWithCFString((CFStringRef)sourceURL);
// 	}
    
// 	JSValueRef exception = NULL;
// 	JSEvaluateScript(jsGlobalContext, scriptJS, NULL, sourceURLJS, 0, &exception );
// 	[self logException:exception ctx:jsGlobalContext];
	
// 	JSStringRelease( scriptJS );
    
// 	if ( sourceURLJS ) {
// 		JSStringRelease( sourceURLJS );
// 	}
// }

// - (JSValueRef)loadModuleWithId:(NSString *)moduleId module:(JSValueRef)module exports:(JSValueRef)exports {
// 	NSString *path = [moduleId stringByAppendingString:@".js"];
// 	NSString *script = [NSString stringWithContentsOfFile:[self pathForResource:path]
// 		encoding:NSUTF8StringEncoding error:NULL];
	
// 	if( !script ) {
// 		NSLog(@"Error: Can't Find Module %@", moduleId );
// 		return NULL;
// 	}
	
// 	NSLog(@"Loading Module: %@", moduleId );
	
// 	JSStringRef scriptJS = JSStringCreateWithCFString((CFStringRef)script);
// 	JSStringRef pathJS = JSStringCreateWithCFString((CFStringRef)path);
// 	JSStringRef parameterNames[] = {
// 		JSStringCreateWithUTF8CString("module"),
// 		JSStringCreateWithUTF8CString("exports"),
// 	};
	
// 	JSValueRef exception = NULL;
// 	JSObjectRef func = JSObjectMakeFunction(jsGlobalContext, NULL, 2, parameterNames, scriptJS, pathJS, 0, &exception );
	
// 	JSStringRelease( scriptJS );
// 	JSStringRelease( pathJS );
// 	JSStringRelease(parameterNames[0]);
// 	JSStringRelease(parameterNames[1]);
	
// 	if( exception ) {
// 		[self logException:exception ctx:jsGlobalContext];
// 		return NULL;
// 	}
	
// 	JSValueRef params[] = { module, exports };
// 	return [self invokeCallback:func thisObject:NULL argc:2 argv:params];
// }

// - (JSValueRef)invokeCallback:(JSObjectRef)callback thisObject:(JSObjectRef)thisObject argc:(size_t)argc argv:(const JSValueRef [])argv {
// 	if( !jsGlobalContext ) { return NULL; } // May already have been released
	
// 	JSValueRef exception = NULL;
// 	JSValueRef result = JSObjectCallAsFunction(jsGlobalContext, callback, thisObject, argc, argv, &exception );
// 	[self logException:exception ctx:jsGlobalContext];
// 	return result;
// }

// - (void)logException:(JSValueRef)exception ctx:(JSContextRef)ctxp {
// 	if( !exception ) return;
	
// 	JSStringRef jsLinePropertyName = JSStringCreateWithUTF8CString("line");
// 	JSStringRef jsFilePropertyName = JSStringCreateWithUTF8CString("sourceURL");
	
// 	JSObjectRef exObject = JSValueToObject( ctxp, exception, NULL );
// 	JSValueRef line = JSObjectGetProperty( ctxp, exObject, jsLinePropertyName, NULL );
// 	JSValueRef file = JSObjectGetProperty( ctxp, exObject, jsFilePropertyName, NULL );
	
// 	NSLog(
// 		@"%@ at line %@ in %@",
// 		JSValueToNSString( ctxp, exception ),
// 		JSValueToNSString( ctxp, line ),
// 		JSValueToNSString( ctxp, file )
// 	);
	
// 	JSStringRelease( jsLinePropertyName );
// 	JSStringRelease( jsFilePropertyName );
// }


// #pragma mark -
// #pragma mark Run loop

// - (void)run:(CADisplayLink *)sender {
// 	if(isPaused) { return; }
	
// 	// We rather poll for device motion updates at the beginning of each frame instead of
// 	// spamming out updates that will never be seen.
// 	[deviceMotionDelegate triggerDeviceMotionEvents];
	
// 	// Check all timers
// 	[timers update];
	
// 	// Redraw the canvas
// 	self.currentRenderingContext = screenRenderingContext;
// 	[screenRenderingContext present];
// }


// - (void)pause {
// 	if( isPaused ) { return; }
	
// 	[lifecycleDelegate pause];
// 	[displayLink removeFromRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
// 	[screenRenderingContext finish];
// 	isPaused = true;
// }

// - (void)resume {
// 	if( !isPaused ) { return; }
	
// 	[lifecycleDelegate resume];
// 	[EAGLContext setCurrentContext:glCurrentContext];
// 	[displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
// 	isPaused = false;
// }

// - (void)clearCaches {
// 	JSGarbageCollect(jsGlobalContext);
// }

// - (void)setCurrentRenderingContext:(EJCanvasContext *)renderingContext {
// 	if( renderingContext != currentRenderingContext ) {
// 		[currentRenderingContext flushBuffers];
// 		[currentRenderingContext release];
		
// 		// Switch GL Context if different
// 		if( renderingContext && renderingContext.glContext != glCurrentContext ) {
// 			glFlush();
// 			glCurrentContext = renderingContext.glContext;
// 			[EAGLContext setCurrentContext:glCurrentContext];
// 		}
		
// 		[renderingContext prepare];
// 		currentRenderingContext = [renderingContext retain];
// 	}
// }


// #pragma mark -
// #pragma mark Touch handlers

// - (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
// 	[touchDelegate triggerEvent:@"touchstart" all:event.allTouches changed:touches remaining:event.allTouches];
// }

// - (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
// 	NSMutableSet *remaining = [event.allTouches mutableCopy];
// 	[remaining minusSet:touches];
	
// 	[touchDelegate triggerEvent:@"touchend" all:event.allTouches changed:touches remaining:remaining];
// 	[remaining release];
// }

// - (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
// 	[self touchesEnded:touches withEvent:event];
// }

// - (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
// 	[touchDelegate triggerEvent:@"touchmove" all:event.allTouches changed:touches remaining:event.allTouches];
// }


// //TODO: Does this belong in this class?
// #pragma mark
// #pragma mark Timers

// - (JSValueRef)createTimer:(JSContextRef)ctxp argc:(size_t)argc argv:(const JSValueRef [])argv repeat:(BOOL)repeat {
// 	if( argc != 2 || !JSValueIsObject(ctxp, argv[0]) || !JSValueIsNumber(jsGlobalContext, argv[1]) ) {
// 		return NULL;
// 	}
	
// 	JSObjectRef func = JSValueToObject(ctxp, argv[0], NULL);
// 	float interval = JSValueToNumberFast(ctxp, argv[1])/1000;
	
// 	// Make sure short intervals (< 18ms) run each frame
// 	if( interval < 0.018 ) {
// 		interval = 0;
// 	}
	
// 	int timerId = [timers scheduleCallback:func interval:interval repeat:repeat];
// 	return JSValueMakeNumber( ctxp, timerId );
// }

// - (JSValueRef)deleteTimer:(JSContextRef)ctxp argc:(size_t)argc argv:(const JSValueRef [])argv {
// 	if( argc != 1 || !JSValueIsNumber(ctxp, argv[0]) ) return NULL;
	
// 	[timers cancelId:JSValueToNumberFast(ctxp, argv[0])];
// 	return NULL;
// }

// @end
