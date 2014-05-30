#pragma once
#include "NSDictionary.h"

class NS_DLL NSCache : public NSDictionary
{
public:
	int countLimit;

	NSCache();
	~NSCache();

	void setCountLimit(int lim);

	void setObject(NSObject* obj  ,const char* key);
	/* data */
};