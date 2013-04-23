#ifndef __EJ_NONRETAINING_PROXY_H__
#define __EJ_NONRETAINING_PROXY_H__

#include "EJCocoa/NSObject.h"

class EJNonRetainingProxy : public NSObject
{
	void* target;
public:
	EJNonRetainingProxy();
	~EJNonRetainingProxy();

	static EJNonRetainingProxy* proxy(void * target);
};

#endif // __EJ_NONRETAINING_PROXY_H__

// #import <Foundation/Foundation.h>

// // "Weak proxy" to avoid retain loops.
// // Adapted from http://stackoverflow.com/a/13921278/1525473

// @interface EJNonRetainingProxy : NSObject {
// 	id target;
// }

// + (EJNonRetainingProxy *)proxyWithTarget:(id)target;

// @end
