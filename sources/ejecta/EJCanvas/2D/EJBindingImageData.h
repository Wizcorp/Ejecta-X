#ifndef __EJ_BINDINGIMAGE_DATA_H__
#define __EJ_BINDINGIMAGE_DATA_H__

#include "EJBindingBase.h"
#include "EJImageData.h"

class EJBindingImageData : public EJBindingBase {
private:
	JSObjectRef dataArray;
	EJImageData* m_imageData;
public:

	EJBindingImageData(JSContextRef ctx, JSObjectRef obj, EJImageData* imageData);
	EJBindingImageData();
	~EJBindingImageData();
	
	virtual string superclass(){return EJBindingBase::toString();};

	EJImageData* imageData();

	REFECTION_CLASS_IMPLEMENT_DEFINE(EJBindingImageData);

	EJ_BIND_GET_DEFINE( data, ctx );
	EJ_BIND_GET_DEFINE( width, ctx );
	EJ_BIND_GET_DEFINE( height, ctx );
};

#endif //_EJ_BINDINGIMAGE_DATA_H_