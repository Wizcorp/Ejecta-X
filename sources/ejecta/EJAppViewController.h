#ifndef __EJ_JAVASCRIPT_VIEW_H__
#define __EJ_JAVASCRIPT_VIEW_H__

#include "EJCocoa/NSObject.h"

class EJJavaScriptView;

class EJAppViewController : public NSObject
{
	EJJavaScriptView *view;
public:
	EJAppViewController();
	~EJAppViewController();

	void init(const char* path, int w, int h);
	void setScreenSize(int w, int h);
	void run(void);

	void didReceiveMemoryWarning();

	static EJAppViewController* instance();
    static void finalize();

	/* data */
	bool landscapeMode;

};
#endif //__EJ_JAVASCRIPT_VIEW_H__


// #import <Foundation/Foundation.h>

// @interface EJAppViewController : UIViewController {
// 	BOOL landscapeMode;
// }

// @end
