#ifndef __EJ_BINDING_EJECTA_CORE_H__
#define __EJ_BINDING_EJECTA_CORE_H__

#include "EJCocoa/NSString.h"
#include "EJBindingBase.h"
#include "EJCanvas/EJFont.h"

enum {
	kEJCoreAlertViewOpenURL = 1,
	kEJCoreAlertViewGetText
};

class EJBindingEjectaCore : public EJBindingBase
{
	NSString *urlToOpen;
	JSObjectRef getTextCallback;
	NSString *_deviceName;
	
public:
	EJBindingEjectaCore();
	~EJBindingEjectaCore();
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingEjectaCore);

	virtual string superclass(){return EJBindingBase::toString();};

	NSString* deviceName();

	EJ_BIND_FUNCTION_DEFINE(log, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(include, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(loadFont, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(requireModule, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(require, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(openURL, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(getText, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(setTimeout, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(setInterval, ctx, argc, argv );
	EJ_BIND_FUNCTION_DEFINE(clearTimeout, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(clearInterval, ctx, argc, argv );

	EJ_BIND_GET_DEFINE(devicePixelRatio, ctx);
	EJ_BIND_GET_DEFINE(screenWidth, ctx);
	EJ_BIND_GET_DEFINE(screenHeight, ctx);
	EJ_BIND_GET_DEFINE(userAgent, ctx);
	EJ_BIND_GET_DEFINE(language, ctx);
	EJ_BIND_GET_DEFINE(appVersion, ctx);
	EJ_BIND_GET_DEFINE(onLine, ctx);
};

#endif // __EJ_BINDING_EJECTA_CORE_H__
