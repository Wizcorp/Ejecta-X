#include "EJBindingImage.h"
#include "../EJApp.h"


EJBindingImage::EJBindingImage() : EJDrawable(0), path(0), loading(false) {
}

EJBindingImage::~EJBindingImage() {
	if(texture)texture->release();
	if(path)path->release();
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

	NSString * fullPath;
	// If path is a Data URI, a remote URL or a File URI we don't want to prepend resource paths
	if(path->hasPrefix("data:")) {
		NSLOG("Loading Image from Data URI not yet implemented.");
		fullPath = NULL;
	} else if(path->hasPrefix("http:") || path->hasPrefix("https:")) {
		NSLOG("Loading Image from URL not yet implemented.");
		fullPath = NULL;
	} else if(path->hasPrefix("file://")) {
		NSLOG("Loading Image from File URI: %s", path->getCString());
		size_t prefixLength = strlen("file://");
		fullPath = path->substringFromIndex(prefixLength);
	} else if(path->hasPrefix("/")) {
		// This handles '/data/' or '/storage/' but not 'data/' or 'storage/', user has to prepend a '/' for full path
		NSLOG("Loading Image from full path: %s", path->getCString());
		fullPath = path;
	} else {
		NSLOG("Loading Image (lazy): %s", path->getCString());
		fullPath = EJApp::instance()->pathForResource(path);
	}

	EJTexture * tempTex = new EJTexture(fullPath, sharegroup);
	tempTex->autorelease();
	endLoad(tempTex);

}

void EJBindingImage::endLoad(EJTexture * tex) {
	loading = false;
	tex->retain();
	texture = tex;

	if( tex->textureId ) {
		EJBindingEventedBase::triggerEvent(NSStringMake("load"));
	}
	else {
		EJBindingEventedBase::triggerEvent(NSStringMake("error"));
	}
}

EJTexture* EJBindingImage::getTexture() {
	return texture;
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
