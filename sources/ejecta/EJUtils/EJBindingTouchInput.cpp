#include "EJBindingTouchInput.h"

EJBindingTouchInput::EJBindingTouchInput() : tag(0)
{
	EJApp::instance()->touchDelegate = this;
}

EJBindingTouchInput::~EJBindingTouchInput()
{

}

void EJBindingTouchInput::triggerEvent(NSString* name, int x, int y)
{
	EJApp* ejecta = EJApp::instance();
	JSContextRef ctx = ejecta->jsGlobalContext;
	float scaling = ejecta->internalScaling;

	JSValueRef params[3];

	params[0] = JSValueMakeNumber(ctx, ++tag);
	params[1] = JSValueMakeNumber(ctx, x / scaling);
	params[2] = JSValueMakeNumber(ctx, y / scaling);

	EJBindingEventedBase::triggerEvent(name, 3, params);
}

EJ_BIND_EVENT(EJBindingTouchInput, touchstart);
EJ_BIND_EVENT(EJBindingTouchInput, touchend);
EJ_BIND_EVENT(EJBindingTouchInput, touchmove);

REFECTION_CLASS_IMPLEMENT(EJBindingTouchInput);
