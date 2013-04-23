#ifndef __EJ_SHARED_OPENAL_MANAGER_H__
#define __EJ_SHARED_OPENAL_MANAGER_H__

#include "EJCocoa/NSObject.h"
#include "EJCocoa/NSDictionary.h"

// #import <OpenAL/al.h>
// #import <OpenAL/alc.h>

class EJSharedOpenALManager : public NSObject
{
	// ALCcontext *context;
	// ALCdevice *device;
	NSDictionary *buffers;

	static EJSharedOpenALManager* sharedOpenALManager;
public:
	EJSharedOpenALManager();
	~EJSharedOpenALManager();

	static EJSharedOpenALManager* instance();
};

#endif // __EJ_SHARED_OPENAL_MANAGER_H__

// #import <Foundation/Foundation.h>

// #import <OpenAL/al.h>
// #import <OpenAL/alc.h>

// @interface EJSharedOpenALManager : NSObject {
// 	ALCcontext *context;
// 	ALCdevice *device;
// 	NSMutableDictionary *buffers;
// }

// + (EJSharedOpenALManager *)instance;
// @property (readonly, nonatomic) NSMutableDictionary *buffers;

// @end
