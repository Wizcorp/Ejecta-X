#ifdef _WINDOWS
#else
#include <sys/time.h>
#endif

#include "EJTimer.h"

EJTimerCollection::EJTimerCollection() : lastId(0),simpleMutex(false)
{
	timers = new NSDictionary();
}

EJTimerCollection::~EJTimerCollection()
{
	timers->autorelease();
}

void EJTimerCollection::lock()
{
	simpleMutex = true;
}

void EJTimerCollection::unlock()
{
	simpleMutex = false;
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
	if(simpleMutex)
	{
		EJTimer* timer = (EJTimer*)timers->objectForKey(timerId);
		if(timer)timer->active = false;
	}
	else 
	{
		lock();
		timers->removeObjectForKey(timerId);
		unlock();
	}
}

void EJTimerCollection::update()
{
	if(!simpleMutex)
	{
		lock();
		NSDictElement* pElement = NULL;
		NSObject* pObject = NULL;
		NSDICT_FOREACH(timers, pElement)
		{
			pObject = pElement->getObject();
			EJTimer* timer = (EJTimer*)pObject;
			timer->check();
			if( !timer->active ) {
				timers->removeObjectForElememt(pElement);
			}	
		}
		unlock();
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