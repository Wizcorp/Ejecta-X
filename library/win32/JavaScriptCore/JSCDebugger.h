//
//  JSCDebugger.h
//  JavaScriptCore
//
//  Created by Rolando Abarca on 1/26/12.
//  Copyright (c) 2012 Zynga Inc. All rights reserved.
//

#ifndef JavaScriptCore_JSCDebugger_h
#define JavaScriptCore_JSCDebugger_h

namespace JSCDebug {
	class JSCDebugger;
};

typedef enum {
	kDebuggerContinue = 0,
	kDebuggerStep,
	kDebuggerStepInto,
	kDebuggerStepOut
} DebuggerState;

class JSCDebuggerDelegate {
public:
	virtual ~JSCDebuggerDelegate() {};
	virtual void didReachException(const char *scriptURL, JSValueRef exception, int lineNumber) = 0;
	virtual void shouldPauseJS() = 0;
	virtual void didPause(DebuggerState *state) = 0;
};

class JSCDebuggerController {
private:
	JSGlobalContextRef		m_globalContext;
	JSCDebug::JSCDebugger	*m_debugger;
	
public:
	JSCDebuggerController(JSGlobalContextRef globalContext, JSCDebuggerDelegate *delegate);
	~JSCDebuggerController();
	
	long setBreakpoint(const char *scriptURL, int lineNumber);
	void removeBreakpoint(long breakpointId);
	void evaluateInCurrentFrame(const char *script);
};

#endif
