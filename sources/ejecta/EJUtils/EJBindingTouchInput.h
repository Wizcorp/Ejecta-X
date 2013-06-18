#ifndef __EJ_BINDING_TOUCHINPUT_H__
#define __EJ_BINDING_TOUCHINPUT_H__

#include "../EJBindingEventedBase.h"

class EJTouchEvent : public NSObject {
public:
	EJTouchEvent(){
		eventName = new NSString("");
		posX = 0;
		posY = 0;
	};
	EJTouchEvent(const char* name, int x, int y){
		eventName = new NSString(name);
		posX = x;
		posY = y;
	};
	~EJTouchEvent(){
		eventName->release();
	};

	NSString* eventName;
	int posX;
	int posY;
};

class EJBindingTouchInput : public EJBindingEventedBase {

	int tag;
	
public:

	EJBindingTouchInput();
	~EJBindingTouchInput();
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingTouchInput);

	virtual string superclass(){ return EJBindingEventedBase::toString();};

	void triggerEvent(NSString* name, int x, int y);

};

#endif // __EJ_TIMER_H__
