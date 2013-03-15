#ifndef __EJ_BINDING_AUDIO_H__
#define __EJ_BINDING_AUDIO_H__

//#import <Foundation/Foundation.h>
#include "../EJBindingEventedBase.h"

// #import "EJAudioSourceOpenAL.h"
// #import "EJAudioSourceAVAudio.h"

// Max file size of audio effects using OpenAL; beyond that, the AVAudioPlayer is used
#define EJ_AUDIO_OPENAL_MAX_SIZE 512 * 1024 // 512kb


typedef enum {
	kEJAudioPreloadNone,
	kEJAudioPreloadMetadata,
	kEJAudioPreloadAuto
} EJAudioPreload;

static const char * EJAudioPreloadNames[] = {
	"none",
	"metadata",
	"auto"
};


class EJBindingAudio : public EJBindingEventedBase {

	EJAudioPreload preload;
	BOOL loading, playAfterLoad;
	void loadOperation(NSString* fullPath);
	void endLoad(NSObject * src);
	void setPreload(EJAudioPreload preloadp);
public:

	NSString * path;
	NSObject * source;
	
	BOOL loop, ended;
	float volume;

	EJBindingAudio();
	~EJBindingAudio();
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingAudio);

	virtual string superclass(){ return EJBindingEventedBase::toString();};

	void load();
	void setSourcePath(NSString* pathp);

	void init(JSContextRef ctx ,JSObjectRef obj, size_t argc, const JSValueRef argv[]);

	EJ_BIND_FUNCTION_DEFINE(play, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(pause, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(load, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(canPlayType, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(cloneNode, ctx, argc, argv);
	EJ_BIND_GET_DEFINE(loop, ctx);
	EJ_BIND_SET_DEFINE(loop, ctx, value);
	EJ_BIND_GET_DEFINE(volume, ctx);
	EJ_BIND_SET_DEFINE(volume, ctx, value);
	EJ_BIND_GET_DEFINE(currentTime, ctx);
	EJ_BIND_SET_DEFINE(currentTime, ctx, value);
	EJ_BIND_GET_DEFINE(src, ctx);
	EJ_BIND_SET_DEFINE(src, ctx, value);
	EJ_BIND_GET_DEFINE(ended, ctx);

	//EJ_BIND_ENUM_DEFINE(preload, EJAudioPreloadNames, self.preload);

};

#endif //__EJ_BINDING_AUDIO_H__