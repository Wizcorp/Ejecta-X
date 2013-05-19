#ifndef __EJ_BINDING_HTTPREQUEST_H__
#define __EJ_BINDING_HTTPREQUEST_H__

#include "../EJBindingEventedBase.h"
#include "../EJCocoa/NSObject.h"

class CCHttpRequest : public NSObject {
public:
    CCHttpRequest(){}
    virtual ~CCHttpRequest(){}
};

class CCHttpResponse : public NSObject {
public:
    CCHttpResponse(){}
    virtual ~CCHttpResponse(){}
    int getResponseCode(){}
};

class CCHttpClient : public NSObject {
public:
    CCHttpClient(){}
    virtual ~CCHttpClient(){}
};

typedef enum {
	kEJHttpRequestTypeString,
	kEJHttpRequestTypeArrayBuffer,
	kEJHttpRequestTypeBlob,
	kEJHttpRequestTypeDocument,
	kEJHttpRequestTypeJSON,
	kEJHttpRequestTypeText
} EJHttpRequestType;

static const char * EJHttpRequestTypeNames[] = {
	"",
	"arraybuffer",
	"blob",
	"document",
	"json",
	"text"
};

typedef enum {
	kEJHttpRequestStateUnsent = 0,
	kEJHttpRequestStateOpened = 1,
	kEJHttpRequestStateHeadersReceived = 2,
	kEJHttpRequestStateLoading = 3,
	kEJHttpRequestStateDone = 4,
} EJHttpRequestState;

class EJBindingHttpRequest : public EJBindingEventedBase {

	EJHttpRequestType type;
	NSString * method;
	NSString * url;
	BOOL async;
	NSString * user;
	NSString * password;
	int timeout;
	NSDictionary * requestHeaders;

	EJHttpRequestState state;	
	CCHttpClient * connection;
	CCHttpResponse * response;
	unsigned char * responseBody;
	
public:

	EJBindingHttpRequest();
	~EJBindingHttpRequest();
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingHttpRequest);

	virtual string superclass(){ return EJBindingEventedBase::toString();};

	virtual void init(JSContextRef ctx ,JSObjectRef obj, size_t argc, const JSValueRef argv[]);

	void clearConnection();
	void clearRequest();
	int getStatusCode();
	NSString * getResponseText();

	EJ_BIND_FUNCTION_DEFINE(open, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(setRequestHeader, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(abort, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(getAllResponseHeaders, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(getResponseHeader, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(overrideMimeType, ctx, argc, argv);
	EJ_BIND_FUNCTION_DEFINE(send, ctx, argc, argv);

	EJ_BIND_GET_DEFINE(readyState, ctx);
	EJ_BIND_GET_DEFINE(response, ctx);
	EJ_BIND_GET_DEFINE(responseText, ctx);
	EJ_BIND_GET_DEFINE(status, ctx);
	EJ_BIND_GET_DEFINE(statusText, ctx);
	EJ_BIND_GET_DEFINE(timeout, ctx);
	EJ_BIND_SET_DEFINE(timeout, ctx, value);

	EJ_BIND_ENUM_DEFINE(responseType, EJHttpRequestTypeNames, type);
};

#endif // __EJ_BINDING_HTTPREQUEST_H__
