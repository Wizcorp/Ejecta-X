#include "NSCache.h"

NSCache::NSCache(): countLimit(8){	  
}

NSCache::~NSCache(){
	removeAllObjects();
}

void NSCache::setCountLimit(int lim){
	countLimit =lim;
}

void NSCache::setObject(NSObject* obj  ,const char* key){
	NSDictionary::setObject(obj,key);

	int c = count();
	if (c >= countLimit)
	{		
		NSString* first =(NSString*) allKeys()->objectAtIndex(0);
		removeObjectForKey(first->getCString());
	}
}