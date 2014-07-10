#ifndef __EJ_BINDING_TOUCHINPUT_H__
#define __EJ_BINDING_TOUCHINPUT_H__

#include "EJBindingEventedBase.h"

class EJTouchEvent : public NSObject {
public:
	EJTouchEvent(){
		eventName = new NSString("");
		posX = 0;
		posY = 0;
		touchId = 0;
	};
	EJTouchEvent(const char* name, int x, int y, int id){
		eventName = new NSString(name);
		posX = x;
		posY = y;
		touchId = id;
	};
	~EJTouchEvent(){
		eventName->release();
	};

	NSString* eventName;
	int touchId;
	int posX;
	int posY;
};

class EJBindingTouchInput : public EJBindingEventedBase {
	
public:

	EJBindingTouchInput();
	~EJBindingTouchInput();
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingTouchInput);

	virtual string superclass(){ return EJBindingEventedBase::toString();};

	void triggerEvent(NSString* name, int x, int y, int id);

};

#endif // __EJ_TIMER_H__
