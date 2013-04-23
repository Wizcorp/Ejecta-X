#ifndef __EJ_SHARED_TEXTURECACHE_VIEW_H__
#define __EJ_SHARED_TEXTURECACHE_VIEW_H__

#include "EJCocoa/NSObject.h"
#include "EJCocoa/NSDictionary.h"

class EJSharedTextureCache : public NSObject
{
	NSDictionary *textures;
	unsigned char *premultiplyTable;
	unsigned char *unPremultiplyTable;
	
	static EJSharedTextureCache* sharedTextureCache;
public:
	EJSharedTextureCache();
	~EJSharedTextureCache();

	static EJSharedTextureCache* instance();
};

#endif //__EJ_SHARED_TEXTURECACHE_VIEW_H__

// #import <Foundation/Foundation.h>

// @interface EJSharedTextureCache : NSObject {
// 	NSMutableDictionary *textures;
// 	NSMutableData *premultiplyTable;
// 	NSMutableData *unPremultiplyTable;
// }

// + (EJSharedTextureCache *)instance;

// @property (nonatomic, readonly) NSMutableDictionary *textures;
// @property (nonatomic, readonly) NSData *premultiplyTable;
// @property (nonatomic, readonly) NSData *unPremultiplyTable;

// @end