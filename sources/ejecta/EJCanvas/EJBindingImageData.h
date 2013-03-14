#ifndef __EJ_BINDINGIMAGE_DATA_H__
#define __EJ_BINDINGIMAGE_DATA_H__

#include "../EJBindingBase.h"
#include "EJImageData.h"

class EJBindingImageData : public EJBindingBase {
private:
	JSObjectRef dataArray;
public:
	EJImageData* m_imageData;

	EJBindingImageData(JSContextRef ctx, JSObjectRef obj, EJImageData* imageData);
	EJBindingImageData();
	~EJBindingImageData();
	
	virtual string superclass(){return EJBindingBase::toString();};

	EJImageData* imageData();
	virtual void init(JSContextRef ctx, JSObjectRef obj, EJImageData* imageData);

	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingImageData);

	EJ_BIND_GET_DEFINE( m_imageData, ctx );
	EJ_BIND_GET_DEFINE( width, ctx );
	EJ_BIND_GET_DEFINE( height, ctx );
};

#endif //_EJ_BINDINGIMAGE_DATA_H_