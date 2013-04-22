#include "EJBindingAudio.h"

EJBindingAudio::EJBindingAudio() {
}

void EJBindingAudio::init(JSContextRef ctx ,JSObjectRef obj, size_t argc, const JSValueRef argv[]) {
	volume = 1;
	preload = kEJAudioPreloadNone;
		
	if( argc > 0 ) {
		setSourcePath(JSValueToNSString(ctx, argv[0]));
	}
}

EJBindingAudio::~EJBindingAudio() {
	//source->release();
	//path->release();
}



void EJBindingAudio::setSourcePath(NSString* pathp) {
	// if( !path || ![path isEqualToString:pathp] ) {
	// 	[path release];
	// 	[source release];
	// 	source = NULL;
		
	// 	path = [pathp retain];
	// 	if( preload == kEJAudioPreloadAuto ) {
	// 		[self load];
	// 	}
	// }
}

void EJBindingAudio::load() {
	// if( source || !path || loading ) { return; }
	
	// // This will begin loading the sound in a background thread
	// loading = YES;
	
	// NSString * fullPath = [[EJApp instance] pathForResource:path];
	// NSInvocationOperation* loadOp = [[NSInvocationOperation alloc] initWithTarget:self
	// 			selector:@selector(loadOperation:) object:fullPath];
	// [loadOp setThreadPriority:0.0];
	// [[EJApp instance].opQueue addOperation:loadOp];
	// [loadOp release];
}

void EJBindingAudio::loadOperation(NSString* fullPath) {
	// NSAutoreleasePool *autoreleasepool = [[NSAutoreleasePool alloc] init];
	
	// // Decide whether to load the sound as OpenAL or AVAudioPlayer source
	// unsigned long long size = [[[NSFileManager defaultManager] attributesOfItemAtPath:fullPath error:nil] fileSize];
	
	// NSObject<EJAudioSource> * src;
	// if( size <= EJ_AUDIO_OPENAL_MAX_SIZE ) {
	// 	NSLog(@"Loading Sound(OpenAL): %@", path);
	// 	src = [[EJAudioSourceOpenAL alloc] initWithPath:fullPath];
	// }
	// else {
	// 	NSLog(@"Loading Sound(AVAudio): %@", path);
	// 	src = [[EJAudioSourceAVAudio alloc] initWithPath:fullPath];
	// 	((EJAudioSourceAVAudio *)src).delegate = self;
	// }
	// [src autorelease];
	
	// [self performSelectorOnMainThread:@selector(endLoad:) withObject:src waitUntilDone:NO];
	// [autoreleasepool release];
}

void EJBindingAudio::endLoad(NSObject * src) {
	// source = [src retain];
	// [source setLooping:loop];
	// [source setVolume:volume];
	
	// if( playAfterLoad ) {
	// 	[source play];
	// }
	// loading = NO;
	
	// [self triggerEvent:@"canplaythrough" argc:0 argv:NULL];
}

void EJBindingAudio::setPreload(EJAudioPreload preloadp) {
	// preload = preloadp;
	// if( preload == kEJAudioPreloadAuto ) {
	// 	[self load];
	// }
}



EJ_BIND_FUNCTION(EJBindingAudio, play, ctx, argc, argv) {
	if( !source ) {
		playAfterLoad = true;
		load();
	}
	else {
		//[source play];
		ended = false;
	}
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingAudio, pause, ctx, argc, argv) {
	if( !source ) {
		playAfterLoad = false;
	}
	else {
		//[source pause];
	}
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingAudio, load, ctx, argc, argv) {
	load();
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingAudio, canPlayType, ctx, argc, argv) {
	if( argc != 1 ) return NULL;
	
	NSString * mime = JSValueToNSString(ctx, argv[0]);
	if( 
		std::string(mime->getCString()).find("audio/x-caf") == std::string::npos || 
		std::string(mime->getCString()).find("audio/mpeg") == std::string::npos || 
		std::string(mime->getCString()).find("audio/mp4") == std::string::npos
	) {
		return NSStringToJSValue(ctx, NSStringMake("probably"));
	}
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingAudio, cloneNode, ctx, argc, argv) {
	// Create new JS object
	EJBindingBase* pClass = (EJBindingBase*)NSClassFromString(std::string("EJBindingAudio"));
	JSClassRef audioClass = EJApp::instance()->getJSClassForClass(pClass);
	JSObjectRef obj = JSObjectMake( ctx, audioClass, NULL );
	
	// Create the native instance
	EJBindingAudio* audio = (EJBindingAudio*)NSClassFromString(pClass->toString().c_str());
	audio->init(ctx, obj, 0, NULL);
	
	audio->loop = loop;
	audio->volume = volume;
	audio->preload = preload;
	audio->path = path;
	
	if( source ) {
		// If the source for this audio element was loaded,
		// load it for the cloned one as well.
		audio->load();
	}
	
	// Attach the native instance to the js object
	JSObjectSetPrivate( obj, (void *)audio );
	pClass->autorelease();
	return obj;
}

EJ_BIND_GET(EJBindingAudio, loop, ctx) {
	return JSValueMakeBoolean( ctx, loop );
}

EJ_BIND_SET(EJBindingAudio, loop, ctx, value) {
	loop = JSValueToBoolean(ctx, value);
	//[source setLooping:loop];
}

EJ_BIND_GET(EJBindingAudio, volume, ctx) {
	return JSValueMakeNumber( ctx, volume );
}

EJ_BIND_SET(EJBindingAudio, volume, ctx, value) {
	volume = MIN(1,MAX(JSValueToNumberFast(ctx, value),0));
	//[source setVolume:volume];
}

EJ_BIND_GET(EJBindingAudio, currentTime, ctx) {
	return 0;//JSValueMakeNumber( ctx, source ? [source getCurrentTime] : 0 );
}

EJ_BIND_SET(EJBindingAudio, currentTime, ctx, value) {
	load();
	//[source setCurrentTime:JSValueToNumberFast(ctx, value)];
}

EJ_BIND_GET(EJBindingAudio, src, ctx) {
	return path ? NSStringToJSValue(ctx, path) : NULL;
}

EJ_BIND_SET(EJBindingAudio, src, ctx, value) {
	setSourcePath(JSValueToNSString(ctx, value));
	if( preload == kEJAudioPreloadAuto ) {
		load();
	}
}

EJ_BIND_GET(EJBindingAudio, ended, ctx) {
	return JSValueMakeBoolean(ctx, ended);
}

//EJ_BIND_ENUM(EJBindingAudio, preload, EJAudioPreloadNames, self.preload);

EJ_BIND_EVENT(EJBindingAudio, canplaythrough);
EJ_BIND_EVENT(EJBindingAudio, ended);



REFECTION_CLASS_IMPLEMENT(EJBindingAudio);