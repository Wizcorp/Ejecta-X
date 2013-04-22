#ifndef __EJ_TIMER_H__
#define __EJ_TIMER_H__

#include <JavaScriptCore/JavaScriptCore.h>

#include "EJCocoa/NSObject.h"
#include "EJCocoa/NSDictionary.h"

class EJJavaScriptView;

class EJTimerCollection : public NSObject
{

	NSDictionary *timers;
	int lastId;

	EJJavaScriptView *scriptView;

	bool simpleMutex;
	void lock();
	void unlock();

public:

	EJTimerCollection(EJJavaScriptView* scriptView);
	~EJTimerCollection();

	int scheduleCallback(JSObjectRef callback, float interval, bool repeat);
	void cancelId(int timerId);
	void update();
};

class EJTimer : public NSObject
{
	double target;
	//NSTimeInterval target;
	float interval;
	JSObjectRef callback;
	bool repeat;
	EJJavaScriptView *scriptView;
public:
	bool active;
	EJTimer();
	EJTimer(EJJavaScriptView* scriptViewp, JSObjectRef callbackp, float intervalp, bool repeatp);
	~EJTimer();

	void check();
};

#endif // __EJ_TIMER_H__
// #import <Foundation/Foundation.h>
// #import <JavaScriptCore/JavaScriptCore.h>

// @class EJJavaScriptView;

// @interface EJTimerCollection : NSObject {
// 	NSMutableDictionary *timers;
// 	int lastId;
// 	EJJavaScriptView *scriptView;
// }

// - (id)initWithScriptView:(EJJavaScriptView *)scriptView;
// - (int)scheduleCallback:(JSObjectRef)callback interval:(NSTimeInterval)interval repeat:(BOOL)repeat;
// - (void)cancelId:(int)timerId;
// - (void)update;

// @end


// @interface EJTimer : NSObject {
// 	NSTimeInterval interval;
// 	JSObjectRef callback;
// 	BOOL active, repeat;
// 	EJJavaScriptView *scriptView;
// }

// - (id)initWithScriptView:(EJJavaScriptView *)scriptViewp
// 	callback:(JSObjectRef)callbackp
// 	interval:(NSTimeInterval)intervalp
// 	repeat:(BOOL)repeatp;
// - (void)check;

// @property (readonly) BOOL active;

// @end
