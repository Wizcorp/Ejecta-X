#ifndef __EJ_BINDING_HTTPREQUEST_H__
#define __EJ_BINDING_HTTPREQUEST_H__

#include "../EJBindingEventedBase.h"
#include "../EJCocoa/NSObject.h"
#include <vector>


class EJHttpClient;
class EJHttpRequest;
class EJHttpResponse;

typedef void (NSObject::*SEL_HttpResponse)(EJHttpClient* client, EJHttpResponse* response);
#define httpresponse_selector(_SELECTOR) (SEL_HttpResponse)(&_SELECTOR)

class EJHttpRequest : public NSObject {
public:
    /** Use this enum type as param in setReqeustType(param) */
    typedef enum
    {
        kHttpGet,
        kHttpPost,
        kHttpPut,
        kHttpDelete,
        kHttpUnkown,
    } HttpRequestType;
    
    /** Constructor 
        Because HttpRequest object will be used between UI thead and network thread,
        requestObj->autorelease() is forbidden to avoid crashes in EJAutoreleasePool
        new/retain/release still works, which means you need to release it manually
        Please refer to HttpRequestTest.cpp to find its usage
     */
    EJHttpRequest()
    {
        _requestType = kHttpUnkown;
        _url.clear();
        _requestData.clear();
        _tag.clear();
        _pTarget = NULL;
        _pSelector = NULL;
        _pUserData = NULL;
    };
    
    /** Destructor */
    virtual ~EJHttpRequest()
    {
        if (_pTarget)
        {
            _pTarget->release();
        }
    };
    
    /** Override autorelease method to avoid developers to call it */
    NSObject* autorelease(void)
    {
        NSAssert(false, "HttpResponse is used between network thread and ui thread \
                 therefore, autorelease is forbidden here");
        return NULL;
    }
            
    // setter/getters for properties
     
    /** Required field for HttpRequest object before being sent.
        kHttpGet & kHttpPost is currently supported
     */
    inline void setRequestType(HttpRequestType type)
    {
        _requestType = type;
    };
    /** Get back the kHttpGet/Post/... enum value */
    inline HttpRequestType getRequestType()
    {
        return _requestType;
    };
    
    /** Required field for HttpRequest object before being sent.
     */
    inline void setUrl(const char* url)
    {
        _url = url;
    };
    /** Get back the setted url */
    inline const char* getUrl()
    {
        return _url.c_str();
    };
    
    /** Option field. You can set your post data here
     */
    inline void setRequestData(const char* buffer, unsigned int len)
    {
        _requestData.assign(buffer, buffer + len);
    };
    /** Get the request data pointer back */
    inline char* getRequestData()
    {
        return &(_requestData.front());
    }
    /** Get the size of request data back */
    inline int getRequestDataSize()
    {
        return _requestData.size();
    }
    
    /** Option field. You can set a string tag to identify your request, this tag can be found in HttpResponse->getHttpRequest->getTag()
     */
    inline void setTag(const char* tag)
    {
        _tag = tag;
    };
    /** Get the string tag back to identify the request. 
        The best practice is to use it in your MyClass::onMyHttpRequestCompleted(sender, HttpResponse*) callback
     */
    inline const char* getTag()
    {
        return _tag.c_str();
    };
    
    /** Option field. You can attach a customed data in each request, and get it back in response callback.
        But you need to new/delete the data pointer manully
     */
    inline void setUserData(void* pUserData)
    {
        _pUserData = pUserData;
    };
    /** Get the pre-setted custom data pointer back.
        Don't forget to delete it. HttpClient/HttpResponse/HttpRequest will do nothing with this pointer
     */
    inline void* getUserData()
    {
        return _pUserData;
    };
    
    /** Required field. You should set the callback selector function at ack the http request completed
     */
    inline void setResponseCallback(NSObject* pTarget, SEL_CallFuncND pSelector)
    {
        setResponseCallback(pTarget, (SEL_HttpResponse) pSelector);
    }

    inline void setResponseCallback(NSObject* pTarget, SEL_HttpResponse pSelector)
    {
        _pTarget = pTarget;
        _pSelector = pSelector;
        
        if (_pTarget)
        {
            _pTarget->retain();
        }
    }    
    /** Get the target of callback selector funtion, mainly used by EJHttpClient */
    inline NSObject* getTarget()
    {
        return _pTarget;
    }

    /* This sub class is just for migration SEL_CallFuncND to SEL_HttpResponse, 
       someday this way will be removed */
    class _prxy
    {
    public:
        _prxy( SEL_HttpResponse cb ) :_cb(cb) {}
        ~_prxy(){};
        operator SEL_HttpResponse() const { return _cb; }
        operator SEL_CallFuncND()   const { return (SEL_CallFuncND) _cb; }
    protected:
        SEL_HttpResponse _cb;
    };
    
    /** Get the selector function pointer, mainly used by EJHttpClient */
    inline _prxy getSelector()
    {
        return _prxy(_pSelector);
    }
    
    /** Set any custom headers **/
    inline void setHeaders(std::vector<std::string> pHeaders)
   	{
   		_headers=pHeaders;
   	}
   
    /** Get custom headers **/
   	inline std::vector<std::string> getHeaders()
   	{
   		return _headers;
   	}

protected:
    // properties
    HttpRequestType             _requestType;    /// kHttpRequestGet, kHttpRequestPost or other enums
    std::string                 _url;            /// target url that this request is sent to
    std::vector<char>           _requestData;    /// used for POST
    std::string                 _tag;            /// user defined tag, to identify different requests in response callback
    NSObject*          			_pTarget;        /// callback target of pSelector function
    SEL_HttpResponse            _pSelector;      /// callback function, e.g. MyLayer::onHttpResponse(EJHttpClient *sender, EJHttpResponse * response)
    void*                       _pUserData;      /// You can add your customed data here 
    std::vector<std::string>    _headers;		      /// custom http headers
};

class EJHttpResponse : public NSObject {
public:
    /** Constructor, it's used by EJHttpClient internal, users don't need to create HttpResponse manually
     @param request the corresponding HttpRequest which leads to this response 
     */
    EJHttpResponse(EJHttpRequest* request)
    {
        _pHttpRequest = request;
        if (_pHttpRequest)
        {
            _pHttpRequest->retain();
        }
        
        _succeed = false;
        _responseData.clear();
        _errorBuffer.clear();
    }
    
    /** Destructor, it will be called in EJHttpClient internal,
     users don't need to desturct HttpResponse object manully 
     */
    virtual ~EJHttpResponse()
    {
        if (_pHttpRequest)
        {
            _pHttpRequest->release();
        }
    }
    
    /** Override autorelease method to prevent developers from calling it */
    NSObject* autorelease(void)
    {
        NSAssert(false, "HttpResponse is used between network thread and ui thread \
                        therefore, autorelease is forbidden here");
        return NULL;
    }
    
    // getters, will be called by users
    
    /** Get the corresponding HttpRequest object which leads to this response 
        There's no paired setter for it, coz it's already setted in class constructor
     */
    inline EJHttpRequest* getHttpRequest()
    {
        return _pHttpRequest;
    }
        
    /** To see if the http reqeust is returned successfully,
        Althrough users can judge if (http return code = 200), we want an easier way
        If this getter returns false, you can call getResponseCode and getErrorBuffer to find more details
     */
    inline bool isSucceed()
    {
        return _succeed;
    };
    
    /** Get the http response raw data */
    inline std::vector<char>* getResponseData()
    {
        return &_responseData;
    }

    /** Get the http response errorCode
     *  I know that you want to see http 200 :)
     */
    inline int getResponseCode()
    {
        return _responseCode;
    }

    /** Get the rror buffer which will tell you more about the reason why http request failed
     */
    inline const char* getErrorBuffer()
    {
        return _errorBuffer.c_str();
    }
    
    // setters, will be called by EJHttpClient
    // users should avoid invoking these methods
    
    
    /** Set if the http request is returned successfully,
     Althrough users can judge if (http code == 200), we want a easier way
     This setter is mainly used in EJHttpClient, users mustn't set it directly
     */
    inline void setSucceed(bool value)
    {
        _succeed = value;
    };
    
    
    /** Set the http response raw buffer, is used by EJHttpClient      
     */
    inline void setResponseData(std::vector<char>* data)
    {
        _responseData = *data;
    }
    
    
    /** Set the http response errorCode
     */
    inline void setResponseCode(int value)
    {
        _responseCode = value;
    }
    
    
    /** Set the error buffer which will tell you more the reason why http request failed
     */
    inline void setErrorBuffer(const char* value)
    {
        _errorBuffer.clear();
        _errorBuffer.assign(value);
    };

    
protected:
    
    // properties
    EJHttpRequest*        _pHttpRequest;  /// the corresponding HttpRequest pointer who leads to this response 
    bool                _succeed;       /// to indecate if the http reqeust is successful simply
    std::vector<char>   _responseData;  /// the returned raw data. You can also dump it as a string
    int                 _responseCode;    /// the status code returned from libcurl, e.g. 200, 404
    std::string         _errorBuffer;   /// if _responseCode != 200, please read _errorBuffer to find the reason 
};

class EJHttpClient : public NSObject
{
public:
    /** Return the shared instance **/
    static EJHttpClient *getInstance();
    
    /** Relase the shared instance **/
    static void destroyInstance();
        
    /**
     * Add a get request to task queue
     * @param request a EJHttpRequest object, which includes url, response callback etc.
                      please make sure request->_requestData is clear before calling "send" here.
     * @return NULL
     */
    void send(EJHttpRequest* request);
  
    
    /**
     * Change the connect timeout
     * @param timeout 
     * @return NULL
     */
    inline void setTimeoutForConnect(int value) {_timeoutForConnect = value;};
    
    /**
     * Get connect timeout
     * @return int
     *
     */
    inline int getTimeoutForConnect() {return _timeoutForConnect;}
    
    
    /**
     * Change the download timeout
     * @param value
     * @return NULL
     */
    inline void setTimeoutForRead(int value) {_timeoutForRead = value;};
    

    /**
     * Get download timeout
     * @return int
     */
    inline int getTimeoutForRead() {return _timeoutForRead;};
    
    /** Poll function called from main thread to dispatch callbacks when http requests finished **/
    void dispatchResponseCallbacks(float delta);
        
private:
    EJHttpClient();
    virtual ~EJHttpClient();
    bool init(void);
    
    /**
     * Init pthread mutex, semaphore, and create new thread for http requests
     * @return bool
     */
    bool lazyInitThreadSemphore();
    
private:
    int _timeoutForConnect;
    int _timeoutForRead;
    
    // std::string reqId;
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
	EJHttpClient * connection;
	EJHttpResponse * response;
	char * responseBody;
	
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
	void onHttpRequestCompleted(NSObject *sender, void *data);
	void loadLocalhost();

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
