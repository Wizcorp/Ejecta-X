/****************************************************************************

****************************************************************************/

#ifndef __EJ_APP_H__
#define __EJ_APP_H__

#ifdef _WINDOWS
#include <windows.h>
#include <tchar.h>
#include <gl/glew.h>
#include <gl/gl.h>
#else
#include <jni.h>
#include <android/log.h>
#include <GLES/gl.h>
#endif

#include <string>
#include <set>
#include <JavaScriptCore/JavaScriptCore.h>

#include "EJCocoa/support/nsMacros.h"
#include "EJCocoa/NSDictionary.h"
#include "EJCocoa/NSObject.h"
#include "EJCocoa/NSString.h"
#include "EJCocoa/NSSet.h"
#include "EJCocoa/NSValue.h"

using namespace std;

#define EJECTA_VERSION "0.99"
#define EJECTA_APP_FOLDER "files/build/"

#define EJECTA_BOOT_JS "ejecta.js"
#define EJECTA_MAIN_JS "index.js"

class EJBindingBase;
class EJTimerCollection;
class EJCanvasContext;
class EJCanvasContextScreen;

class EJTouchDelegate {
public:
	virtual void triggerEvent(NSString * name, NSSet * all, NSSet * changed, NSSet * remaining) = 0;
};


class EJApp : public NSObject {

	BOOL paused;

	NSDictionary * jsClasses;
	
	EJTimerCollection * timers;
	long currentTime;

	static EJApp* ejectaInstance;

	char* mainBundle;

	

public:

	BOOL landscapeMode;
	JSGlobalContextRef jsGlobalContext;
	int height, width;

	EJCanvasContext * currentRenderingContext;
	EJCanvasContextScreen * screenRenderingContext;
	float internalScaling;

    EJApp(void);
    ~EJApp(void);

	void init(const char* path, int w, int h);
	void setScreenSize(int w, int h);
	void run(void);
	void pause(void);
	void resume(void);
	void clearCaches(void);
	NSString * pathForResource(NSString * resourcePath);
	JSValueRef createTimer(JSContextRef ctx, size_t argc, const JSValueRef argv[], bool repeat);
	JSValueRef deleteTimer(JSContextRef ctx, size_t argc, const JSValueRef argv[]);

	JSClassRef getJSClassForClass(EJBindingBase* classId);
	void hideLoadingScreen(void);
	void loadScriptAtPath(NSString * path);
	JSValueRef loadModuleWithId(NSString * moduleId, JSValueRef module, JSValueRef exports);
 	JSValueRef invokeCallback(JSObjectRef callback, JSObjectRef thisObject, size_t argc, const JSValueRef argv[]);
	void logException(JSValueRef exception, JSContextRef ctxp);

    static EJApp* instance();
    static void finalize();
	void setCurrentRenderingContext(EJCanvasContext * renderingContext);

};

#endif // __EJ_APP_H__
