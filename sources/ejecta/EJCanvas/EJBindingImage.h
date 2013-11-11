#ifndef __EJ_BINDING_IMAGE_H__
#define __EJ_BINDING_IMAGE_H__

#include "../EJBindingEventedBase.h"
#include "EJDrawable.h"
#include "../EJCocoa/NSString.h"

class EJBindingImage : public EJBindingEventedBase, public EJDrawable {

	NSString* path;
	BOOL loading;

	void beginLoad();
	void load(NSString* sharegroup);
	void endLoad(EJTexture * tex);
public:

	EJBindingImage();
	~EJBindingImage();
	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingImage);

	virtual string superclass(){ return EJBindingEventedBase::toString();};

	virtual EJTexture* getTexture();

	EJ_BIND_GET_DEFINE(src, ctx );
	EJ_BIND_SET_DEFINE(src, ctx, value);
	EJ_BIND_GET_DEFINE(width, ctx );
	EJ_BIND_GET_DEFINE(height, ctx );
	EJ_BIND_GET_DEFINE(complete, ctx );
	
	// EJ_BIND_EVENT_DEFINE(load);
	// EJ_BIND_EVENT_DEFINE(error);
};

#endif //__EJ_BINDING_IMAGE_H__