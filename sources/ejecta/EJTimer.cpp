#ifdef _WINDOWS
#else
#include <sys/time.h>
#endif

#include "EJTimer.h"
#include "EJJavaScriptView.h"

EJTimerCollection::EJTimerCollection(EJJavaScriptView* scriptViewp) : lastId(0),simpleMutex(false)
{
	scriptView = scriptViewp;
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
	
	EJTimer * timer = new EJTimer(scriptView, callback, interval, repeat);
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

EJTimer::EJTimer(EJJavaScriptView* scriptViewp, JSObjectRef callbackp, float intervalp, BOOL repeatp)
{
	scriptView = scriptViewp;
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
	JSValueProtect(scriptView->jsGlobalContext, callback);
}

EJTimer::~EJTimer()
{
	target = 0;
	JSValueUnprotect(scriptView->jsGlobalContext, callback);
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
		scriptView->invokeCallback(callback, NULL, 0, NULL);
		
		if( repeat ) {
			target = currentTime + interval;
		}
		else {
			active = false;
		}
	}
}

// #import "EJTimer.h"
// #import "EJJavaScriptView.h"


// @implementation EJTimerCollection


// - (id)initWithScriptView:(EJJavaScriptView *)scriptViewp {
// 	if (self = [super init]) {
// 		scriptView = scriptViewp;
// 		timers = [[NSMutableDictionary alloc] init];
// 	}
// 	return self;
// }

// - (void)dealloc {
// 	[timers release];
// 	[super dealloc];
// }

// - (int)scheduleCallback:(JSObjectRef)callback interval:(NSTimeInterval)interval repeat:(BOOL)repeat {
// 	lastId++;
	
// 	EJTimer *timer = [[EJTimer alloc] initWithScriptView:scriptView callback:callback interval:interval repeat:repeat];
// 	timers[@(lastId)] = timer;
// 	[timer release];
// 	return lastId;
// }

// - (void)cancelId:(int)timerId {
// 	[timers removeObjectForKey:@(timerId)];
// }

// - (void)update {	
// 	for( NSNumber *timerId in [timers allKeys]) {
// 		EJTimer *timer = [timers[timerId] retain];
// 		[timer check];
		
// 		if( !timer.active ) {
// 			[timers removeObjectForKey:timerId];
// 		}
//         [timer release];
// 	}
// }

// @end



// @interface EJTimer()
// @property (nonatomic, retain) NSDate *target;
// @end


// @implementation EJTimer
// @synthesize active;

// - (id)initWithScriptView:(EJJavaScriptView *)scriptViewp
// 	callback:(JSObjectRef)callbackp
// 	interval:(NSTimeInterval)intervalp
// 	repeat:(BOOL)repeatp
// {
// 	if( self = [super init] ) {
// 		scriptView = scriptViewp;
// 		active = true;
// 		interval = intervalp;
// 		repeat = repeatp;
// 		self.target = [NSDate dateWithTimeIntervalSinceNow:interval];
		
// 		callback = callbackp;
// 		JSValueProtect(scriptView.jsGlobalContext, callback);
// 	}
// 	return self;
// }

// - (void)dealloc {
// 	self.target = nil;
// 	JSValueUnprotectSafe(scriptView.jsGlobalContext, callback);
// 	[super dealloc];
// }

// - (void)check {	
// 	if( active && self.target.timeIntervalSinceNow <= 0 ) {
// 		[scriptView invokeCallback:callback thisObject:NULL argc:0 argv:NULL];
		
// 		if( repeat ) {
// 			self.target = [NSDate dateWithTimeIntervalSinceNow:interval];
// 		}
// 		else {
// 			active = false;
// 		}
// 	}
// }


// @end
