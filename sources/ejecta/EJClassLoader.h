#ifndef __EJ_CLASSLOADER_VIEW_H__
#define __EJ_CLASSLOADER_VIEW_H__

#include <JavaScriptCore/JavaScriptCore.h>

#include "EJCocoa/NSObject.h"
#include "EJCocoa/NSString.h"

class EJJavaScriptView;
class EJBindingBase;

class EJClassLoader : public NSObject
{
 public:
 	EJClassLoader(EJJavaScriptView *scriptView, NSString *name);
 	~EJClassLoader();

	static JSClassRef getJSClass(EJBindingBase* classId);
	static JSClassRef createJSClass (EJBindingBase* classId);

 };

#endif // __EJ_CLASSLOADER_VIEW_H__
