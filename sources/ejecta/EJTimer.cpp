#ifdef _WINDOWS
#else
#include <sys/time.h>
#endif

#include "EJTimer.h"

EJTimerCollection::EJTimerCollection()
{
	timers = NSDictionary::create();
}

EJTimerCollection::~EJTimerCollection()
{
	timers->release();
}

int EJTimerCollection::scheduleCallback(JSObjectRef callback, float interval, BOOL repeat)
{
	lastId++;
	
	EJTimer * timer = new EJTimer(callback, interval, repeat);
	timers->setObject(timer, lastId);
	timer->release();
	return lastId;
}

void EJTimerCollection::cancelId(int timerId)
{
	timers->removeObjectForKey(timerId);
}

void EJTimerCollection::update()
{
	NSDictElement* pElement = NULL;
	NSObject* pObject = NULL;
	NSDICT_FOREACH(timers, pElement)
	{
		pObject = pElement->getObject();
	    EJTimer* timer = (EJTimer*)pObject;
        timer->check();
        if( !timer->active ) {
			timers->removeObjectForKey(pElement->getIntKey());
		}	
	}
}


EJTimer::EJTimer()
{

}

EJTimer::EJTimer(JSObjectRef callbackp, float intervalp, BOOL repeatp)
{
	active = true;
	interval = intervalp;
	repeat = repeatp;

#ifdef _WINDOWS	
	static bool is_first = true;
	static LARGE_INTEGER freq;
	static LARGE_INTEGER now;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&now);
	target =  now.QuadPart * 1000000.0 / freq.QuadPart + interval;
#else
	struct timeval time;
	//struct timezone tz;
	gettimeofday(&time, NULL);
	target = time.tv_sec * 1000000.0 + time.tv_usec  + interval;
#endif

	callback = callbackp;
	JSValueProtect(EJApp::instance()->jsGlobalContext, callback);
}

EJTimer::~EJTimer()
{
	JSValueUnprotect(EJApp::instance()->jsGlobalContext, callback);
}

void EJTimer::check()
{

#ifdef _WINDOWS	
	static bool is_first = true;
	static LARGE_INTEGER freq;
	static LARGE_INTEGER now;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&now);
	double currentTime =  now.QuadPart * 1000000.0 / freq.QuadPart + interval;
#else
	struct timeval time;
	//struct timezone tz;
	gettimeofday(&time, NULL);
	double currentTime = time.tv_sec * 1000000.0 + time.tv_usec + interval;
#endif

	if( active && target <= currentTime) {
		EJApp::instance()->invokeCallback(callback, NULL, 0, NULL);
		
		if( repeat ) {
			target = currentTime + interval;
		}
		else {
			active = false;
		}
	}
}