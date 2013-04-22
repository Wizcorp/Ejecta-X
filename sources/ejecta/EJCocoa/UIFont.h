#pragma once
#include "support/uthash.h"
#include "NSObject.h"
#include "NSString.h"
#include <stdarg.h>


class NS_DLL UIFont : public NSObject {

public:
	float pointSize;
	NSString* fontName;

	UIFont(NSString* fn,float ps):fontName(fn),pointSize(ps){
		if (fontName)fontName->retain();
	}
	~UIFont(){
		if (fontName)fontName->release();
	}
};