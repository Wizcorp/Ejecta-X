#include "EJBindingImage.h"
#include "../EJApp.h"


EJBindingImage::EJBindingImage() : texture(0), path(0), loading(false) {
}

EJBindingImage::~EJBindingImage() {
	texture->release();
	path->release();
}

void EJBindingImage::beginLoad() {
	// This will begin loading the texture in a background thread and will call the
	// JavaScript onload callback when done
	loading = true;
	
	NSString* sharegroup = new NSString();
	sharegroup->autorelease();
	load(sharegroup);
}

void EJBindingImage::load(NSString* sharegroup) {

	NSLOG("Loading Image: %s", path->getCString() );
	NSString * fullPath = EJApp::instance()->pathForResource(path);
	EJTexture * tempTex = new EJTexture(fullPath, sharegroup);
	tempTex->autorelease();
	endLoad(tempTex);

}

void EJBindingImage::endLoad(EJTexture * tex) {
	loading = false;
	tex->retain();
	texture = tex;

	if( tex->textureId ) {
		EJBindingEventedBase::triggerEvent(NSStringMake("load") ,0 ,NULL);
	}
	else {
		EJBindingEventedBase::triggerEvent(NSStringMake("error") ,0 ,NULL);
	}
}

EJ_BIND_GET( EJBindingImage, src, ctx ) { 
	JSStringRef src = JSStringCreateWithUTF8CString( path->getCString() );
	JSValueRef ret = JSValueMakeString(ctx, src);
	JSStringRelease(src);
	return ret;
}

EJ_BIND_SET( EJBindingImage, src, ctx, value) {
	// If the texture is still loading, do nothing to avoid confusion
	// This will break some edge cases; FIXME
	if( loading ) { return; }

	NSString * newPath = JSValueToNSString( ctx, value );
	
	// Release the old path and texture?
	if( path ) {
	
		// Same as the old path? Nothing to do here
		if( path->isEqual(newPath) ) { return; }

		path->release();
		path = NULL;
		
		texture->release();
		texture = NULL;
	}
	
	if( newPath->length() ) {
		newPath->retain();
		path = newPath;
		beginLoad();
	}

}

EJ_BIND_GET( EJBindingImage, width, ctx ) {
	return JSValueMakeNumber( ctx, texture ? (texture->width / texture->contentScale) : 0);
}

EJ_BIND_GET( EJBindingImage, height, ctx ) { 
	return JSValueMakeNumber( ctx, texture ? (texture->height / texture->contentScale) : 0 );
}

EJ_BIND_GET( EJBindingImage, complete, ctx ) {
	return JSValueMakeBoolean(ctx, (texture && texture->textureId) );
}

EJ_BIND_EVENT( EJBindingImage, load);

EJ_BIND_EVENT( EJBindingImage, error);

REFECTION_CLASS_IMPLEMENT(EJBindingImage);