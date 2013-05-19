#include "EJBindingHttpRequest.h"

EJBindingHttpRequest::EJBindingHttpRequest() {
	requestHeaders = new NSDictionary();
}

EJBindingHttpRequest::~EJBindingHttpRequest() {
	requestHeaders->release();
	clearRequest();
	clearConnection();
}

void EJBindingHttpRequest::init(JSContextRef ctx ,JSObjectRef obj, size_t argc, const JSValueRef argv[]) {
}

void EJBindingHttpRequest::clearConnection()
{
	if(connection)connection->release(); connection = NULL;
	//if(responseBody)responseBody->release(); responseBody = NULL;
	if(response)response->release(); response = NULL;
}

void EJBindingHttpRequest::clearRequest()
{
	if(method)method->release(); method = NULL;
	if(url)url->release(); url = NULL;
	if(user)user->release(); user = NULL;
	if(password)password->release(); password = NULL;
}

int EJBindingHttpRequest::getStatusCode()
{
	if( !response ) {
		return 0;
	}
	else if( response->getResponseCode() != 200 ) {
		return response->getResponseCode();
	}
	else {
		return 200; // assume everything went well for non-HTTP resources
	}
}

NSString * EJBindingHttpRequest::getResponseText()
{
	if( !response || !responseBody ) { return NULL; }

	// NSStringEncoding encoding = NSASCIIStringEncoding;
	// if ( response.textEncodingName ) {
	// 	CFStringEncoding cfEncoding = CFStringConvertIANACharSetNameToEncoding((CFStringRef) [response textEncodingName]);
	// 	if( cfEncoding != kCFStringEncodingInvalidId ) {
	// 		encoding = CFStringConvertEncodingToNSStringEncoding(cfEncoding);
	// 	}
	// }

	return NSStringMake("responseBody");
}

EJ_BIND_FUNCTION(EJBindingHttpRequest, open, ctx, argc, argv) {	
	if( argc < 2 ) { return NULL; }

	// Cleanup previous request, if any
	clearConnection();
	clearRequest();

	method = JSValueToNSString( ctx, argv[0] ); method->retain();
	url = JSValueToNSString( ctx, argv[1] ); url->retain();
	async = argc > 2 ? JSValueToBoolean( ctx, argv[2] ) : true;

	if( argc > 4 ) {
		user = JSValueToNSString( ctx, argv[3] ); user->retain();
		password = JSValueToNSString( ctx, argv[4] ); password->retain();
	}

	state = kEJHttpRequestStateOpened;
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingHttpRequest, setRequestHeader, ctx, argc, argv) {
	if( argc < 2 ) { return NULL; }

	NSString * header = JSValueToNSString( ctx, argv[0] );
	NSString * value = JSValueToNSString( ctx, argv[1] );

	requestHeaders->setObject(value, header->getCString());
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingHttpRequest, abort, ctx, argc, argv) {
	if( connection ) {
		clearConnection();
		EJBindingEventedBase::triggerEvent(NSStringMake("abort"), 0, NULL);
	}
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingHttpRequest, getAllResponseHeaders, ctx, argc, argv) {
	if( !response ) {
		return NULL;
	}

	NSString * headers = NSStringMake("");

	return NSStringToJSValue(ctx, headers);
}

EJ_BIND_FUNCTION(EJBindingHttpRequest, getResponseHeader, ctx, argc, argv) {
	if( argc < 1 || !response ) {
		return NULL;
	}

	NSString * header = JSValueToNSString( ctx, argv[0] );
	NSString * value = header;

	return value ? NSStringToJSValue(ctx, value) : NULL;
}

EJ_BIND_FUNCTION(EJBindingHttpRequest, overrideMimeType, ctx, argc, argv) {
	// TODO?
	return NULL;
}

EJ_BIND_FUNCTION(EJBindingHttpRequest, send, ctx, argc, argv) {
	if( !method || !url ) { return NULL; }

	clearConnection();

	// NSURL * requestUrl = [NSURL URLWithString:url];
	// if( !requestUrl.host ) {
	// 	// No host? Assume we have a local file
	// 	requestUrl = [NSURL fileURLWithPath:[[EJApp instance] pathForResource:url]];
	// }
	// NSMutableURLRequest * request = [[NSMutableURLRequest alloc] initWithURL:requestUrl];
	// [request setHTTPMethod:method];

	// for( NSString * header in requestHeaders ) {
	// 	[request setValue:[requestHeaders objectForKey:header] forHTTPHeaderField:header];
	// }

	// if( argc > 0 ) {
	// 	NSString * requestBody = JSValueToNSString( ctx, argv[0] );
	// 	NSData * requestData = [NSData dataWithBytes:[requestBody UTF8String] length:[requestBody length]];
	// 	[request setHTTPBody:requestData];
	// }

	// if( timeout ) {
	// 	NSTimeInterval timeoutSeconds = (float)timeout/1000.0f;
	// 	[request setTimeoutInterval:timeoutSeconds];
	// }	

	// NSLog(@"XHR: %@ %@", method, url);
	// [self triggerEvent:@"loadstart" argc:0 argv:NULL];

	// if( async ) {
	// 	state = kEJHttpRequestStateLoading;
	// 	connection = [[NSURLConnection alloc] initWithRequest:request delegate:self];
	// }
	// else {	
	// 	NSData * data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:nil];
	// 	responseBody = [[NSMutableData alloc] initWithData:data];
	// 	[response retain];

	// 	state = kEJHttpRequestStateDone;
	// 	if( [response isKindOfClass:[NSHTTPURLResponse class]] ) {
	// 		NSHTTPURLResponse * urlResponse = (NSHTTPURLResponse *)response;
	// 		if( urlResponse.statusCode == 200 ) {
	// 			[self triggerEvent:@"load" argc:0 argv:NULL];
	// 		}
	// 	}
	// 	else {
	// 		[self triggerEvent:@"load" argc:0 argv:NULL];
	// 	}
	// 	[self triggerEvent:@"loadend" argc:0 argv:NULL];
	// 	[self triggerEvent:@"readystatechange" argc:0 argv:NULL];
	// }
	// [request release];

	return NULL;
}

EJ_BIND_GET(EJBindingHttpRequest, readyState, ctx) {
	return JSValueMakeNumber( ctx, state );
}

EJ_BIND_GET(EJBindingHttpRequest, response, ctx) {
	NSString * responseText = getResponseText();
	if( !responseText ) { return NULL; }

	// if( type == kEJHttpRequestTypeJSON ) {
	// 	JSStringRef jsText = JSStringCreateWithCFString((CFStringRef)responseText);
	// 	JSObjectRef jsonObject = (JSObjectRef)JSValueMakeFromJSONString(ctx, jsText);
	// 	JSStringRelease(jsText);
	// 	return jsonObject;
	// }
	// else {
		return NSStringToJSValue( ctx, responseText );
	//}
}

EJ_BIND_GET(EJBindingHttpRequest, responseText, ctx) {
	NSString * responseText = getResponseText();	
	return responseText ? NSStringToJSValue( ctx, responseText ) : NULL;
}

EJ_BIND_GET(EJBindingHttpRequest, status, ctx) {
	return JSValueMakeNumber( ctx, getStatusCode() );
}

EJ_BIND_GET(EJBindingHttpRequest, statusText, ctx) {
	// FIXME: should be "200 OK" instead of just "200"

 	NSString * code = new NSString();
 	code->autorelease();
 	code->initWithFormat("%d",getStatusCode());
	return NSStringToJSValue(ctx, code);
}

EJ_BIND_GET(EJBindingHttpRequest, timeout, ctx) {
	return JSValueMakeNumber( ctx, timeout );
}

EJ_BIND_SET(EJBindingHttpRequest, timeout, ctx, value) {
	timeout = JSValueToNumberFast( ctx, value );
}

//EJ_BIND_ENUM(EJBindingHttpRequest, responseType, EJHttpRequestTypeNames, type);

EJ_BIND_CONST(UNSENT, kEJHttpRequestStateUnsent);
EJ_BIND_CONST(OPENED, kEJHttpRequestStateOpened);
EJ_BIND_CONST(HEADERS_RECEIVED, kEJHttpRequestStateHeadersReceived);
EJ_BIND_CONST(LOADING, kEJHttpRequestStateLoading);
EJ_BIND_CONST(DONE, kEJHttpRequestStateDone);

EJ_BIND_EVENT(EJBindingHttpRequest, readystatechange);
EJ_BIND_EVENT(EJBindingHttpRequest, loadend);
EJ_BIND_EVENT(EJBindingHttpRequest, load);
EJ_BIND_EVENT(EJBindingHttpRequest, error);
EJ_BIND_EVENT(EJBindingHttpRequest, abort);
EJ_BIND_EVENT(EJBindingHttpRequest, progress);
EJ_BIND_EVENT(EJBindingHttpRequest, loadstart);
EJ_BIND_EVENT(EJBindingHttpRequest, timeout);

REFECTION_CLASS_IMPLEMENT(EJBindingHttpRequest);
