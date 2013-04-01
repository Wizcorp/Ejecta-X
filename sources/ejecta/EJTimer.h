#ifndef __EJ_TIMER_H__
#define __EJ_TIMER_H__

#include "EJApp.h"
#include "EJCocoa/NSObject.h"
#include "EJCocoa/NSDictionary.h"

class EJTimerCollection : public NSObject
{

	NSDictionary * timers;
	int lastId;
	boolean simpleMutex;
	void lock();
	void unlock();

public:

	EJTimerCollection();
	~EJTimerCollection();

	int scheduleCallback(JSObjectRef callback, float interval, BOOL repeat);
	void cancelId(int timerId);
	void update();
};

class EJTimer : public NSObject
{
	double target;
	//NSTimeInterval target;
	float interval;
	JSObjectRef callback;
	BOOL repeat;
public:
	BOOL active;
	EJTimer();
	EJTimer(JSObjectRef callbackp, float intervalp, BOOL repeatp);
	~EJTimer();

	void check();
};

#endif // __EJ_TIMER_H__