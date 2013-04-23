#ifndef __EJ_JAVASCRIPT_VIEW_H__
#define __EJ_JAVASCRIPT_VIEW_H__

#include "EJCocoa/NSString.h"
// #import <MediaPlayer/MediaPlayer.h>

typedef enum {
	kEJVideoScalingModeNone,
	kEJVideoScalingModeAspectFit,
	kEJVideoScalingModeAspectFill,
	kEJVideoScalingModeFill
} EJVideoScalingMode;

class EJBindingVideo : public EJBindingEventedBase
{
	NSString *path;
	bool loaded;
	bool showControls;
	EJVideoScalingMode scalingMode;
	//MPMoviePlayerController *player;

public:
	EJBindingVideo();
	~EJBindingVideo();
};


// #import "EJBindingEventedBase.h"
// #import <MediaPlayer/MediaPlayer.h>

// typedef enum {
// 	kEJVideoScalingModeNone,
// 	kEJVideoScalingModeAspectFit,
// 	kEJVideoScalingModeAspectFill,
// 	kEJVideoScalingModeFill
// } EJVideoScalingMode;

// @interface EJBindingVideo : EJBindingEventedBase <UIGestureRecognizerDelegate> {
// 	NSString *path;
// 	BOOL loaded;
// 	BOOL showControls;
// 	EJVideoScalingMode scalingMode;
// 	MPMoviePlayerController *player;
// }

// @end
