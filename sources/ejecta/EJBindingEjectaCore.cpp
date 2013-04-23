#include "EJBindingEjectaCore.h"
#include "EJJavaScriptView.h"

EJBindingEjectaCore::EJBindingEjectaCore() : urlToOpen(0), getTextCallback(0),_deviceName(0)
{

}

EJBindingEjectaCore::~EJBindingEjectaCore()
{
	if (urlToOpen)urlToOpen->release();
	if( getTextCallback ) {
		JSValueUnprotect(scriptView->jsGlobalContext, getTextCallback);
	}
}

NSString* EJBindingEjectaCore::deviceName()
{
	
	NSString *machine = new NSString("Android");
	machine->autorelease();
	
	if( machine->compare("i386") ||
	    machine->compare("x86_64") ) {
		
		NSString *deviceType = NSStringMake("Android Simulator");
		deviceType->autorelease();

		_deviceName = deviceType;
		_deviceName->retain();
		return deviceType;
		
	} else {
		_deviceName = machine;
		_deviceName->retain();
		return machine;
	}
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, log, ctx, argc, argv ) {
	if( argc < 1 ) return NULL;
	
	NSLOG("JS: %s", JSValueToNSString(ctx, argv[0])->getCString());
	return NULL;
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, include, ctx, argc, argv ) {
	if( argc < 1 ) { return NULL; }

	scriptView->loadScriptAtPath(JSValueToNSString(ctx, argv[0]));
	return NULL;
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, loadFont, ctx, argc, argv ) {
	if( argc < 1 ) { return NULL; }

	NSString *path = JSValueToNSString(ctx, argv[0]);
	NSString *fullPath = scriptView->pathForResource(path);
	//EJFont->loadFontAtPath(fullPath);
	return NULL;
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, requireModule, ctx, argc, argv ) {
	if( argc < 3 ) { return NULL; }
	
	return scriptView->loadModule(JSValueToNSString(ctx, argv[0]) ,argv[1],argv[2]);
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, require, ctx, argc, argv ) {
	// TODO: remove entirely for next release
	if( argc < 1 ) { return NULL; }
	NSLog("Warning: ejecta.require() is deprecated. Use ejecta.include() instead.");
	
	scriptView->loadScriptAtPath(JSValueToNSString(ctx, argv[0]));
	return NULL;
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, openURL, ctx, argc, argv ) {
	if( argc < 1 ) { return NULL; }
	
	NSLOG("openURL() Not Implemented. return NULL");
	// NSString *url = JSValueToNSString( ctx, argv[0] );
	// if( argc == 2 ) {
	// 	[urlToOpen release];
	// 	urlToOpen = [url retain];
		
	// 	NSString *confirm = JSValueToNSString( ctx, argv[1] );
	// 	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Open Browser?" message:confirm delegate:self cancelButtonTitle:@"Cancel" otherButtonTitles:@"Ok", nil];
	// 	alert.tag = kEJCoreAlertViewOpenURL;
	// 	[alert show];
	// 	[alert release];
	// }
	// else {
	// 	[[UIApplication sharedApplication] openURL:[NSURL URLWithString: url]];
	// }
	return NULL;
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, getText, ctx, argc, argv) {
	if( argc < 3 ) { return NULL; }
	
	NSLOG("getText() Not Implemented. return NULL");
	// NSString *title = JSValueToNSString(ctx, argv[0]);
	// NSString *message = JSValueToNSString(ctx, argv[1]);
	
	// JSValueUnprotectSafe(ctx, getTextCallback);
	// getTextCallback = JSValueToObject(ctx, argv[2], NULL);
	// JSValueProtect(ctx, getTextCallback);
	
	// UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:self
	// 	cancelButtonTitle:@"Cancel" otherButtonTitles:@"Ok", nil];
	// alert.alertViewStyle = UIAlertViewStylePlainTextInput;
	// alert.tag = kEJCoreAlertViewGetText;
	// [alert show];
	// [alert release];
	return NULL;
}

// - (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)index {
// 	if( alertView.tag == kEJCoreAlertViewOpenURL ) {
// 		if( index == 1 ) {
// 			[[UIApplication sharedApplication] openURL:[NSURL URLWithString:urlToOpen]];
// 		}
// 		[urlToOpen release];
// 		urlToOpen = nil;
// 	}
	
// 	else if( alertView.tag == kEJCoreAlertViewGetText ) {
// 		NSString *text = @"";
// 		if( index == 1 ) {
// 			text = [[alertView textFieldAtIndex:0] text];
// 		}
// 		JSValueRef params[] = { NSStringToJSValue(scriptView.jsGlobalContext, text) };
// 		[scriptView invokeCallback:getTextCallback thisObject:NULL argc:1 argv:params];
		
// 		JSValueUnprotectSafe(scriptView.jsGlobalContext, getTextCallback);
// 		getTextCallback = NULL;
// 	}
// }


EJ_BIND_FUNCTION( EJBindingEjectaCore, setTimeout, ctx, argc, argv ) {
	return scriptView->createTimer(ctx,argc,argv, false);
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, setInterval, ctx, argc, argv ) {
	return scriptView->createTimer(ctx ,argc,argv,true);
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, clearTimeout, ctx, argc, argv ) {
	return scriptView->deleteTimer(ctx,argc,argv);
}

EJ_BIND_FUNCTION( EJBindingEjectaCore, clearInterval, ctx, argc, argv ) {
	return scriptView->deleteTimer(ctx ,argc,argv);
}


EJ_BIND_GET( EJBindingEjectaCore, devicePixelRatio, ctx ) {
	//return JSValueMakeNumber( ctx, [UIScreen mainScreen].scale );
	return JSValueMakeNumber( ctx, 1);
}

EJ_BIND_GET( EJBindingEjectaCore, screenWidth, ctx ) {
	return JSValueMakeNumber( ctx, scriptView->width );
}

EJ_BIND_GET( EJBindingEjectaCore, screenHeight, ctx ) {
	return JSValueMakeNumber( ctx, scriptView->height);
}

EJ_BIND_GET( EJBindingEjectaCore, userAgent, ctx ) {	
	NSString* userAgentString = NSStringMake("");
	userAgentString->initWithFormat("Ejecta/%s (%s;)", EJECTA_VERSION, deviceName()->getCString());
	return NSStringToJSValue(
		ctx,
		userAgentString
	);
}

EJ_BIND_GET( EJBindingEjectaCore, language, ctx) {
	return NSStringToJSValue( ctx, NSStringMake("undefine") );
}

EJ_BIND_GET( EJBindingEjectaCore, appVersion, ctx ) {
	return NSStringToJSValue( ctx, NSStringMake(EJECTA_VERSION) );
}

EJ_BIND_GET( EJBindingEjectaCore, onLine, ctx) {

	NSLOG("onLine Not Implemented.and return true");
	// struct sockaddr_in zeroAddress;
	// bzero(&zeroAddress, sizeof(zeroAddress));
	// zeroAddress.sin_len = sizeof(zeroAddress);
	// zeroAddress.sin_family = AF_INET;
	
	// SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithAddress(
	// 	kCFAllocatorDefault,
	// 	(const struct sockaddr*)&zeroAddress
	// );
	// if( reachability ) {
	// 	SCNetworkReachabilityFlags flags;
	// 	SCNetworkReachabilityGetFlags(reachability, &flags);
		
	// 	CFRelease(reachability);
		
	// 	if(
	// 		// Reachable and no connection required
	// 		(
	// 			(flags & kSCNetworkReachabilityFlagsReachable) &&
	// 			!(flags & kSCNetworkReachabilityFlagsConnectionRequired)
	// 		) ||
	// 		// or connection can be established without user intervention
	// 		(
	// 			(flags & kSCNetworkReachabilityFlagsConnectionOnDemand) &&
	// 			(flags & kSCNetworkReachabilityFlagsConnectionOnTraffic) &&
	// 			!(flags & kSCNetworkReachabilityFlagsInterventionRequired)
	// 		)
	// 	) {
	// 		return JSValueMakeBoolean(ctx, true);
	// 	}
	// }
	
	return JSValueMakeBoolean(ctx, false);
}

REFECTION_CLASS_IMPLEMENT(EJBindingEjectaCore);