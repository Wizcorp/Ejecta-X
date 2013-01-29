#include "EJBindingImageData.h"

EJBindingImageData::EJBindingImageData(JSContextRef ctx, JSObjectRef obj, EJImageData* imageData):EJBindingBase(ctx, obj, 0 ,NULL){
	// EJBindingBase::EJBindingBase(ctx, obj, 0 ,NULL);
	// m_imageData = imageData->retain();
	imageData->retain();
	m_imageData = imageData;
	dataArray = NULL;
}

EJBindingImageData::EJBindingImageData():EJBindingBase() {
	// EJBindingBase::EJBindingBase();
	dataArray = NULL;
}

void EJBindingImageData::init(JSContextRef ctx, JSObjectRef obj, EJImageData* imageData) {
	EJBindingBase::init(ctx, obj, 0, NULL);
	// m_imageData = imageData->retain();
	imageData->retain();
	m_imageData = imageData;
}

EJBindingImageData::~EJBindingImageData() {
	JSContextRef ctx = EJApp::instance()->jsGlobalContext;
	if (dataArray) {
		JSValueUnprotect(ctx, dataArray);
	}
	m_imageData->release();
}

EJImageData* EJBindingImageData::imageData() {
	if( dataArray ) {
		// Copy values from the JSArray back into the imageData
		JSContextRef ctx = EJApp::instance()->jsGlobalContext;
		int count = m_imageData->width * m_imageData->height * 4;
		
		JSObjectToByteArray(ctx, dataArray, m_imageData->pixels, count );
	}
	return m_imageData;
}

EJ_BIND_GET( EJBindingImageData, m_imageData, ctx ) {
	if( !dataArray ) {
		int count = m_imageData->width * m_imageData->height * 4;
		dataArray = ByteArrayToJSObject(ctx, m_imageData->pixels, count);
		JSValueProtect(ctx, dataArray);
	}
	return dataArray;
}

EJ_BIND_GET( EJBindingImageData, width, ctx ) {
	return JSValueMakeNumber( ctx, m_imageData->width);
}

EJ_BIND_GET( EJBindingImageData, height, ctx ) { 
	return JSValueMakeNumber( ctx, m_imageData->height );
}

REFECTION_CLASS_IMPLEMENT(EJBindingImageData);