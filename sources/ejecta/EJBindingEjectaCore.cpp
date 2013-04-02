#include "EJBindingEjectaCore.h"
#include "EJConvert.h"


EJBindingEjectaCore::EJBindingEjectaCore() 
{

}

EJBindingEjectaCore::~EJBindingEjectaCore()
{
	delete urlToOpen;
	if( getTextCallback ) {
		JSValueUnprotect(EJApp::instance()->jsGlobalContext, getTextCallback);
	}
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, log, ctx, argc, argv ) {
	if( argc < 1 ) return NULL;
	
	NSLOG("JS: %s", JSValueToNSString(ctx, argv[0])->getCString());
	return NULL;
}
//
EJ_BIND_FUNCTION(EJBindingEjectaCore,include, ctx, argc, argv ) {
	if( argc < 1 ) { return NULL; }

	EJApp::instance()->loadScriptAtPath(JSValueToNSString(ctx, argv[0]));
	return NULL;
}
//
EJ_BIND_FUNCTION(EJBindingEjectaCore,requireModule, ctx, argc, argv ) {
	if( argc < 3 ) { return NULL; }

	return EJApp::instance()->loadModuleWithId(JSValueToNSString(ctx, argv[0]) ,argv[1],argv[2]);
}
//
EJ_BIND_FUNCTION(EJBindingEjectaCore,require, ctx, argc, argv ) {
	// TODO: remove entirely for next release
	if( argc < 1 ) { return NULL; }
	NSLOG("Warning: ejecta.require() is deprecated. Use ejecta.include() instead.");

	EJApp::instance()->loadScriptAtPath(JSValueToNSString(ctx, argv[0]));
	return NULL;
}
//
EJ_BIND_FUNCTION(EJBindingEjectaCore,openURL, ctx, argc, argv ) {

	NSLOG("openURL() Not Implemented 。 return NULL");
	return NULL;
}
//
EJ_BIND_FUNCTION(EJBindingEjectaCore,getText, ctx, argc, argv) {

	NSLOG("getText() Not Implemented。return NULL");
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingEjectaCore,setTimeout, ctx, argc, argv ) {
	return EJApp::instance()->createTimer(ctx,argc,argv, false);
}
//
EJ_BIND_FUNCTION(EJBindingEjectaCore,setInterval, ctx, argc, argv ) {
	return EJApp::instance()->createTimer(ctx ,argc,argv,true);
}
//
EJ_BIND_FUNCTION(EJBindingEjectaCore,clearTimeout, ctx, argc, argv ) {
	return EJApp::instance()->deleteTimer(ctx,argc,argv);
}
//
EJ_BIND_FUNCTION(EJBindingEjectaCore,clearInterval, ctx, argc, argv ) {
	return EJApp::instance()->deleteTimer(ctx ,argc,argv);
}
//

EJ_BIND_GET(EJBindingEjectaCore,devicePixelRatio, ctx ) {
	return JSValueMakeNumber( ctx, 1);
}
//
EJ_BIND_GET(EJBindingEjectaCore,screenWidth, ctx ) {
	return JSValueMakeNumber( ctx, EJApp::instance()->width );
}
//
EJ_BIND_GET(EJBindingEjectaCore,screenHeight, ctx ) {
	return JSValueMakeNumber( ctx, EJApp::instance()->height);
}
//
EJ_BIND_GET(EJBindingEjectaCore,landscapeMode, ctx ) {
	return JSValueMakeBoolean( ctx, EJApp::instance()->landscapeMode );
}
//
EJ_BIND_GET(EJBindingEjectaCore,userAgent, ctx ) {
	return NSStringToJSValue(ctx,NSStringMake("android"));
}
//
EJ_BIND_GET(EJBindingEjectaCore,appVersion, ctx ) {
	return NSStringToJSValue( ctx, NSStringMake(EJECTA_VERSION) );
}
//
EJ_BIND_GET(EJBindingEjectaCore,onLine, ctx) {
	NSLOG("onLine Not Implemented.and return true");
	return JSValueMakeBoolean(ctx, true);
}

REFECTION_CLASS_IMPLEMENT(EJBindingEjectaCore);