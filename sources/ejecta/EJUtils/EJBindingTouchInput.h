#ifndef __EJ_BINDING_TOUCHINPUT_H__
#define __EJ_BINDING_TOUCHINPUT_H__

#include "../EJBindingEventedBase.h"


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
