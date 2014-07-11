#include "EJBindingCanvas.h"
#include "EJBindingImageData.h"
#include "EJCanvasContextScreen.h"
#include "EJCanvasPattern.h"
#include "EJBindingCanvasPattern.h"

bool EJBindingCanvas::firstCanvasInstance = true;

EJBindingCanvas::EJBindingCanvas() : renderingContext(0), isScreenCanvas(false) {
	
	ejectaInstance = EJApp::instance();
	scalingMode = kEJScalingModeFitWidth;
	useRetinaResolution = true;
	msaaEnabled = false;
	msaaSamples = 2;	

	if( firstCanvasInstance ) {
		isScreenCanvas = true;
		width = EJApp::instance()->width;
		height = EJApp::instance()->height;
	} else {
		width = 0;
		height = 0;
	}

}

void EJBindingCanvas::initWithContext(JSContextRef ctx, JSObjectRef obj, size_t argc, const JSValueRef argv[]) {
	EJBindingBase::initWithContext(ctx, obj, 0, NULL);
	ejectaInstance = EJApp::instance();
	scalingMode = kEJScalingModeFitWidth;
	useRetinaResolution = true;
	msaaEnabled = false;
	msaaSamples = 2;

	if( firstCanvasInstance ) {
		isScreenCanvas = true;
		firstCanvasInstance = false;
	}
	
	if( argc == 2 ) {
		width = (short)JSValueToNumberFast(ctx, argv[0]);
		height = (short)JSValueToNumberFast(ctx, argv[1]);
	}
	else {
		if(isScreenCanvas) {
			width = EJApp::instance()->width;
			height = EJApp::instance()->height;
		} else {
			width = 0;
			height = 0;
		}
	}
}

EJBindingCanvas::EJBindingCanvas(JSContextRef ctx ,JSObjectRef obj, size_t argc, const JSValueRef argv[]) : renderingContext(0), isScreenCanvas(false) {
	
	ejectaInstance = EJApp::instance();
	scalingMode = kEJScalingModeFitWidth;
	useRetinaResolution = true;
	msaaEnabled = false;
	msaaSamples = 2;

	if( firstCanvasInstance ) {
		isScreenCanvas = true;
		firstCanvasInstance = false;
	}
	
	if( argc == 2 ) {
		width = (short)JSValueToNumberFast(ctx, argv[0]);
		height = (short)JSValueToNumberFast(ctx, argv[1]);
	}
	else {
		width = EJApp::instance()->width;
		height = EJApp::instance()->height;
	}
}

EJBindingCanvas::~EJBindingCanvas() {
	if(renderingContext) {
		renderingContext->release();
	}
}

EJTexture* EJBindingCanvas::getTexture() {
	if (renderingContext && strcmp(renderingContext->getClassName(),"EJCanvasContextTexture") == 0){
		return ((EJCanvasContextTexture *)renderingContext)->getTexture();
	}
	else {
		return NULL;
	}
}

EJ_BIND_ENUM_GETTER( EJBindingCanvas, globalCompositeOperation, EJCompositeOperation, renderingContext->getGlobalCompositeOperation);
EJ_BIND_ENUM_SETTER( EJBindingCanvas, globalCompositeOperation, EJCompositeOperation, renderingContext->setGlobalCompositeOperation);
EJ_BIND_ENUM( EJBindingCanvas, lineCap, EJLineCap, renderingContext->state->lineCap);
EJ_BIND_ENUM( EJBindingCanvas, lineJoin, EJLineJoin, renderingContext->state->lineJoin);
EJ_BIND_ENUM( EJBindingCanvas, textAlign, EJTextAlign, renderingContext->state->textAlign);
EJ_BIND_ENUM( EJBindingCanvas, textBaseline, EJTextBaseline, renderingContext->state->textBaseline);
EJ_BIND_ENUM( EJBindingCanvas, scalingMode, EJScalingMode, scalingMode);

EJ_BIND_GET( EJBindingCanvas, fillStyle, ctx ) {
	return ColorRGBAToJSValue(ctx, renderingContext->state->fillColor);
}

EJ_BIND_SET( EJBindingCanvas, fillStyle, ctx, value) {
	if( JSValueIsObject(ctx, value) )
	{
		// Try CanvasPattern or CanvasGradient
		EJFillable *fillable;

		if( (fillable = EJBindingCanvasPattern::patternFromJSValue(value)) )
		{
			renderingContext->setFillObject(fillable);
		}
		/*
		else if( (fillable = [EJBindingCanvasGradient gradientFromJSValue:value]) ) {
			renderingContext.fillObject = fillable;
		}
		*/
	}
	else
	{
		// Should be a color string
		renderingContext->state->fillColor = JSValueToColorRGBA(ctx, value);
		renderingContext->setFillObject(NULL);
	}
}

EJ_BIND_GET( EJBindingCanvas, strokeStyle, ctx ) {
	return ColorRGBAToJSValue(ctx, renderingContext->state->strokeColor);
}

EJ_BIND_SET( EJBindingCanvas, strokeStyle, ctx, value) {
	renderingContext->state->strokeColor = JSValueToColorRGBA(ctx, value);
}

EJ_BIND_GET( EJBindingCanvas, globalAlpha, ctx ) {
	return JSValueMakeNumber(ctx, renderingContext->state->globalAlpha );
}

EJ_BIND_SET( EJBindingCanvas, globalAlpha, ctx, value) {
	double maxValue = MAX(JSValueToNumberFast(ctx, value),0);
	renderingContext->state->globalAlpha = MIN(1, maxValue);
}

EJ_BIND_GET( EJBindingCanvas, lineWidth, ctx) {
	return JSValueMakeNumber(ctx, renderingContext->state->lineWidth);
}

EJ_BIND_SET( EJBindingCanvas, lineWidth, ctx, value) {
	renderingContext->state->lineWidth = (float)JSValueToNumberFast(ctx, value);
}

EJ_BIND_GET( EJBindingCanvas, miterLimit, ctx) {
	return JSValueMakeNumber(ctx, renderingContext->state->miterLimit);
}

EJ_BIND_SET( EJBindingCanvas, miterLimit, ctx, value) {
	renderingContext->state->miterLimit = (float)JSValueToNumberFast(ctx, value);
}

EJ_BIND_GET( EJBindingCanvas,font, ctx) {
	UIFont * font = renderingContext->state->font;
 	NSString * name = NSString::createWithFormat("%pt %s", (int)font->pointSize, font->fontName->getCString());
	name->autorelease();
 	return NSStringToJSValue(ctx, name);
}

EJ_BIND_SET( EJBindingCanvas,font, ctx, value) {
 	char string[64]; // Long font names are long
 	JSStringRef jsString = JSValueToStringCopy( ctx, value, NULL );
 	JSStringGetUTF8CString(jsString, string, 64);

 	// Yeah, oldschool!
 	float size = 0;
 	char name[64];
 	sscanf( string, "%fp%*[tx] %63s", &size, name); // matches: 10.5p[tx] helvetica
	
 	UIFont * newFont = new UIFont(NSStringMake(name),size);
 	if( newFont ) {
		if(renderingContext->state->font)
			renderingContext->state->font->release();
 	 	renderingContext->state->font = newFont;
 	}

 	JSStringRelease(jsString);
}

 EJ_BIND_GET( EJBindingCanvas,width, ctx) {
 	return JSValueMakeNumber(ctx, width);
 }

 EJ_BIND_SET( EJBindingCanvas,width, ctx, value) {
 	short newWidth = JSValueToNumberFast(ctx, value);
	if( renderingContext ) {
		ejectaInstance->setCurrentRenderingContext(renderingContext);
		renderingContext->setWidth(newWidth);
		width = renderingContext->getWidth();
		return;
	}
 	width = newWidth;
 }

 EJ_BIND_GET( EJBindingCanvas,height, ctx) {
 	return JSValueMakeNumber(ctx, height);
 }

 EJ_BIND_SET( EJBindingCanvas,height, ctx, value) {
 	short newHeight = JSValueToNumberFast(ctx, value);
	if( renderingContext ) {
		ejectaInstance->setCurrentRenderingContext(renderingContext);
		renderingContext->setHeight(newHeight);
		height = renderingContext->getHeight();
		return;
	}
 	height = newHeight;
 }

EJ_BIND_GET(EJBindingCanvas, offsetLeft, ctx) {
	return JSValueMakeNumber(ctx, 0);
}

EJ_BIND_GET(EJBindingCanvas, offsetTop, ctx) {
	return JSValueMakeNumber(ctx, 0);
}

EJ_BIND_SET(EJBindingCanvas, retinaResolutionEnabled, ctx, value) {
	useRetinaResolution = JSValueToBoolean(ctx, value);
}

EJ_BIND_GET(EJBindingCanvas, retinaResolutionEnabled, ctx) {
	return JSValueMakeBoolean(ctx, useRetinaResolution);
}

EJ_BIND_SET(EJBindingCanvas, imageSmoothingEnabled, ctx, value) {
	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);
	renderingContext->imageSmoothingEnabled = JSValueToBoolean(ctx, value);
}

EJ_BIND_GET(EJBindingCanvas, imageSmoothingEnabled, ctx) {
	return JSValueMakeBoolean(ctx, renderingContext->imageSmoothingEnabled);
}

EJ_BIND_GET(EJBindingCanvas, backingStorePixelRatio, ctx) {
	return JSValueMakeNumber(ctx, renderingContext->backingStoreRatio);
}

EJ_BIND_SET(EJBindingCanvas, MSAAEnabled, ctx, value) {
	msaaEnabled = JSValueToBoolean(ctx, value);
}

EJ_BIND_GET(EJBindingCanvas, MSAAEnabled, ctx) {
	return JSValueMakeBoolean(ctx, msaaEnabled);
}

EJ_BIND_SET(EJBindingCanvas, MSAASamples, ctx, value) {
	int samples = (int)JSValueToNumberFast(ctx, value);
	if( samples == 2 || samples == 4 ) {
		msaaSamples	= samples;
	}
}

EJ_BIND_GET(EJBindingCanvas, MSAASamples, ctx) {
	return JSValueMakeNumber(ctx, msaaSamples);
}
//

EJ_BIND_FUNCTION( EJBindingCanvas, getContext, ctx, argc, argv) {
	if( argc < 1 || !JSValueToNSString(ctx, argv[0])->isEqual(NSStringMake("2d")) ) { 
		return NULL; 
	};
	
	if( renderingContext ) { return jsObject; }
	ejectaInstance->setCurrentRenderingContext(NULL);
	if( isScreenCanvas ) {
		EJCanvasContextScreen * sc = new EJCanvasContextScreen(width, height);
		sc->useRetinaResolution = useRetinaResolution;
		sc->scalingMode = scalingMode;
		
		ejectaInstance->screenRenderingContext = sc;		
		renderingContext = (EJCanvasContext*)sc;
	}
	else {
		renderingContext = (EJCanvasContext*)(new EJCanvasContextTexture(width, height));
	}
	
	renderingContext->msaaEnabled = msaaEnabled;
	renderingContext->msaaSamples = msaaSamples;
	
	renderingContext->create();
	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);

	// Context and canvas are one and the same object, so getContext just
	// returns itself
	return jsObject;
}

//start
// EJ_BIND_FUNCTION(getContext, ctx, argc, argv) {
// 	if( argc < 1 || ![JSValueToNSString(ctx, argv[0]) isEqualToString:@"2d"] ) { 
// 		return NULL; 
// 	};
	
// 	if( renderingContext ) { return jsObject; }
// 	ejectaInstance->currentRenderingContext = nil;
		
// 	if( isScreenCanvas ) {
// 		EJCanvasContextScreen * sc = [[EJCanvasContextScreen alloc] initWithWidth:width height:height];
// 		sc.useRetinaResolution = useRetinaResolution;
// 		sc.scalingMode = scalingMode;
		
// 		ejectaInstance->screenRenderingContext = sc;		
// 		renderingContext = sc;
// 	}
// 	else {
// 		renderingContext = [[EJCanvasContextTexture alloc] initWithWidth:width height:height];
// 	}
	
// 	renderingContext->msaaEnabled = msaaEnabled;
// 	renderingContext->msaaSamples = msaaSamples;
	
// 	[renderingContext create];
// 	ejectaInstance->currentRenderingContext = renderingContext;

// 	// Context and canvas are one and the same object, so getContext just
// 	// returns itself
// 	return jsObject;
// }

 EJ_BIND_FUNCTION(EJBindingCanvas,save, ctx, argc, argv) {
 	renderingContext->save();
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,restore, ctx, argc, argv) {
	 renderingContext->restore();
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,rotate, ctx, argc, argv) {
 	if( argc < 1 ) { return NULL; }
 	renderingContext->rotate(JSValueToNumberFast(ctx, argv[0]));
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,translate, ctx, argc, argv) {
 	if( argc < 2 ) { return NULL; }
 	renderingContext->translate(JSValueToNumberFast(ctx, argv[0]),JSValueToNumberFast(ctx, argv[1]) );
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,scale, ctx, argc, argv) {
 	if( argc < 2 ) { return NULL; }
 	renderingContext->scale(JSValueToNumberFast(ctx, argv[0]),JSValueToNumberFast(ctx, argv[1]));
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,transform, ctx, argc, argv) {
 	if( argc < 6 ) { return NULL; }
	
 	float
 		m11 = JSValueToNumberFast(ctx, argv[0]),
 		m12 = JSValueToNumberFast(ctx, argv[1]),
 		m21 = JSValueToNumberFast(ctx, argv[2]),
 		m22 = JSValueToNumberFast(ctx, argv[3]),
 		dx = JSValueToNumberFast(ctx, argv[4]),
 		dy = JSValueToNumberFast(ctx, argv[5]);
 	renderingContext->transform(m11,m12,m21,m22,dx,dy);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,setTransform, ctx, argc, argv) {
 	if( argc < 6 ) { return NULL; }
	
 	float
 		m11 = JSValueToNumberFast(ctx, argv[0]),
 		m12 = JSValueToNumberFast(ctx, argv[1]),
 		m21 = JSValueToNumberFast(ctx, argv[2]),
 		m22 = JSValueToNumberFast(ctx, argv[3]),
 		dx = JSValueToNumberFast(ctx, argv[4]),
 		dy = JSValueToNumberFast(ctx, argv[5]);
 	renderingContext->setTransform(m11,m12,m21 ,m22 ,dx ,dy);
 	return NULL;
 }

EJ_BIND_FUNCTION(EJBindingCanvas, drawImage, ctx, argc, argv) {

	if( argc < 3 || !JSValueIsObject(ctx, argv[0]) ) return NULL;

	EJDrawable* drawable = NULL;

	//Compare the JSObject to find out if it's an image or a canvas
	//TODO: Buffer temporary data to avoid creation at each call?
 	EJBindingImage* tempBindingImage = new EJBindingImage();
 	JSClassRef imageClass = EJApp::instance()->getJSClassForClass((EJBindingImage*)tempBindingImage);
 	if(JSValueIsObjectOfClass(ctx, argv[0], imageClass)) {
 		drawable = (EJBindingImage*)JSObjectGetPrivate((JSObjectRef)argv[0]);
 	} else {
	 	EJBindingCanvas* tempBindingCanvas = new EJBindingCanvas();
	 	JSClassRef imageClass = EJApp::instance()->getJSClassForClass((EJBindingCanvas*)tempBindingCanvas);
	 	if(JSValueIsObjectOfClass(ctx, argv[0], imageClass)) {
 			drawable = (EJBindingCanvas*)JSObjectGetPrivate((JSObjectRef)argv[0]);
	 	}
	 	delete tempBindingCanvas;
 	}
 	delete tempBindingImage;

 	if(drawable == NULL) {
 		return NULL;
 	}

	// NSObject<EJDrawable> * drawable = (NSObject<EJDrawable> *)JSObjectGetPrivate((JSObjectRef)argv[0]);
	EJTexture * image = drawable->getTexture();

	float scale = image?image->contentScale:1;
	
	short sx = 0, sy = 0, sw = 0, sh = 0;
	float dx = 0, dy = 0, dw = sw, dh = sh;	
	
	if( argc == 3 ) {
		// drawImage(image, dx, dy)
		dx = (float)JSValueToNumberFast(ctx, argv[1]);
		dy = (float)JSValueToNumberFast(ctx, argv[2]);
		sw = image?image->width:0;
		sh = image?image->height:0;
		dw = sw / scale;
		dh = sh / scale;
	}
	else if( argc == 5 ) {
		// drawImage(image, dx, dy, dw, dh)
		dx = (float)JSValueToNumberFast(ctx, argv[1]);
		dy = (float)JSValueToNumberFast(ctx, argv[2]);
		dw = (float)JSValueToNumberFast(ctx, argv[3]);
		dh = (float)JSValueToNumberFast(ctx, argv[4]);
		sw = image?image->width:0;
		sh = image?image->height:0;
	}
	else if( argc >= 9 ) {
		// drawImage(image, sx, sy, sw, sh, dx, dy, dw, dh)
		sx = (short)(JSValueToNumberFast(ctx, argv[1]) * scale);
		sy = (short)(JSValueToNumberFast(ctx, argv[2]) * scale);
		sw = (short)(JSValueToNumberFast(ctx, argv[3]) * scale);
		sh = (short)(JSValueToNumberFast(ctx, argv[4]) * scale);
		
		dx = (float)JSValueToNumberFast(ctx, argv[5]);
		dy = (float)JSValueToNumberFast(ctx, argv[6]);
		dw = (float)JSValueToNumberFast(ctx, argv[7]);
		dh = (float)JSValueToNumberFast(ctx, argv[8]);
	}
	else {
		return NULL;
	}
	
	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);
	renderingContext->drawImage(image, sx, sy, sw, sh, dx, dy, dw, dh);
	// [renderingContext drawImage:image sx:sx sy:sy sw:sw sh:sh dx:dx dy:dy dw:dw dh:dh];

	return NULL;
}

 EJ_BIND_FUNCTION(EJBindingCanvas,fillRect, ctx, argc, argv) {
 	if( argc < 4 ) { return NULL; }
	
 	float
 		dx = JSValueToNumberFast(ctx, argv[0]),
 		dy = JSValueToNumberFast(ctx, argv[1]),
 		w = JSValueToNumberFast(ctx, argv[2]),
 		h = JSValueToNumberFast(ctx, argv[3]);
		
 	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);
 	renderingContext->fillRect(dx,dy,w,h);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,strokeRect, ctx, argc, argv) {
 	if( argc < 4 ) { return NULL; }
	
 	float
 		dx = JSValueToNumberFast(ctx, argv[0]),
 		dy = JSValueToNumberFast(ctx, argv[1]),
 		w = JSValueToNumberFast(ctx, argv[2]),
 		h = JSValueToNumberFast(ctx, argv[3]);
	
	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);
 	renderingContext->strokeRect(dx,dy,w,h);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,clearRect, ctx, argc, argv) {
 	if( argc < 4 ) { return NULL; }
	
 	float
 		dx = JSValueToNumberFast(ctx, argv[0]),
 		dy = JSValueToNumberFast(ctx, argv[1]),
 		w = JSValueToNumberFast(ctx, argv[2]),
 		h = JSValueToNumberFast(ctx, argv[3]);
	
	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);
 	renderingContext->clearRect(dx,dy,w,h);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,getImageData, ctx, argc, argv) {
 	if( argc < 4 ) { return NULL; }
	
 	float
 		sx = JSValueToNumberFast(ctx, argv[0]),
 		sy = JSValueToNumberFast(ctx, argv[1]),
 		sw = JSValueToNumberFast(ctx, argv[2]),
 		sh = JSValueToNumberFast(ctx, argv[3]);
		
 	// Get the image data
 	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);
 	EJImageData * imageData = renderingContext->getImageData(sx,sy,sw,sh);
	
 	// Create the JS object
 	EJBindingImageData* tempData = new EJBindingImageData();
 	JSClassRef imageDataClass = EJApp::instance()->getJSClassForClass((EJBindingBase*)tempData);
 	tempData->autorelease();
 	JSObjectRef obj = JSObjectMake( ctx, imageDataClass, NULL );
 	JSValueProtect(ctx, obj);

 	// Create the native instance
 	EJBindingImageData * jsImageData = new EJBindingImageData(ctx,obj,imageData);
	
 	// Attach the native instance to the js object
 	JSObjectSetPrivate( obj, (void *)jsImageData );
 	JSValueUnprotect(ctx, obj);
 	return obj;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,createImageData, ctx, argc, argv) {
 	if( argc < 2 ) { return NULL; }
	
 	float
 		sw = JSValueToNumberFast(ctx, argv[0]),
 		sh = JSValueToNumberFast(ctx, argv[1]);
		
 	GLubyte * pixels = (GLubyte *)calloc( sw * sh * 4, sizeof(GLubyte) );
 	EJImageData * imageData = new EJImageData(sw ,sh ,pixels);
 	imageData->autorelease();

 	// Create the JS object
 	EJBindingImageData* tempData = new EJBindingImageData();
 	JSClassRef imageDataClass = EJApp::instance()->getJSClassForClass((EJBindingBase*)tempData);
 	delete tempData;
 	JSObjectRef obj = JSObjectMake( ctx, imageDataClass, NULL );
 	JSValueProtect(ctx, obj);

 	// Create the native instance
 	EJBindingImageData * jsImageData =new EJBindingImageData(ctx,obj,imageData);

 	// Attach the native instance to the js object
 	JSObjectSetPrivate( obj, (void *)jsImageData );
 	JSValueUnprotect(ctx, obj);
 	return obj;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas,putImageData, ctx, argc, argv) {
 	if( argc < 3 ) { return NULL; }
	
 	EJBindingImageData * jsImageData = (EJBindingImageData *)JSObjectGetPrivate((JSObjectRef)argv[0]);
 	float
 		dx = JSValueToNumberFast(ctx, argv[1]),
 		dy = JSValueToNumberFast(ctx, argv[2]);
	
 	//ejectaInstance->currentRenderingContext = renderingContext;

	ejectaInstance->setCurrentRenderingContext(renderingContext);
 	renderingContext->putImageData(jsImageData->imageData(), dx, dy);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, beginPath, ctx, argc, argv ) {
 	renderingContext->beginPath();
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, closePath, ctx, argc, argv ) {
 	renderingContext->closePath();
 	return NULL;
 }

 EJ_BIND_FUNCTION( EJBindingCanvas,fill, ctx, argc, argv ) {
 	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);
 	renderingContext->fill();
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, stroke, ctx, argc, argv ) {
 	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);
 	renderingContext->stroke();
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, moveTo, ctx, argc, argv ) {
 	if( argc < 2 ) { return NULL; }
	
 	float
 		x = JSValueToNumberFast(ctx, argv[0]),
 		y = JSValueToNumberFast(ctx, argv[1]);
 	renderingContext->moveTo(x,y);
	
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, lineTo, ctx, argc, argv ) {
 	if( argc < 2 ) { return NULL; }
	
 	float
 		x = JSValueToNumberFast(ctx, argv[0]),
 		y = JSValueToNumberFast(ctx, argv[1]);
 	renderingContext->lineTo(x,y);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, rect, ctx, argc, argv ) {
 	if( argc < 4 ) { return NULL; }
	
 	float
 		x = JSValueToNumberFast(ctx, argv[0]),
 		y = JSValueToNumberFast(ctx, argv[1]),
 		w = JSValueToNumberFast(ctx, argv[2]),
 		h = JSValueToNumberFast(ctx, argv[3]);
 	renderingContext->rect(x,y,w ,h);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, bezierCurveTo, ctx, argc, argv ) {
 	if( argc < 6 ) { return NULL; }
	
 	float
 		cpx1 = JSValueToNumberFast(ctx, argv[0]),
 		cpy1 = JSValueToNumberFast(ctx, argv[1]),
 		cpx2 = JSValueToNumberFast(ctx, argv[2]),
 		cpy2 = JSValueToNumberFast(ctx, argv[3]),
 		x = JSValueToNumberFast(ctx, argv[4]),
 		y = JSValueToNumberFast(ctx, argv[5]);
 	renderingContext->bezierCurveTo(cpx1,cpy1 ,cpx2,cpy2,x,y);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, quadraticCurveTo, ctx, argc, argv ) {
 	if( argc < 4 ) { return NULL; }
	
 	float
 		cpx = JSValueToNumberFast(ctx, argv[0]),
 		cpy = JSValueToNumberFast(ctx, argv[1]),
 		x = JSValueToNumberFast(ctx, argv[2]),
 		y = JSValueToNumberFast(ctx, argv[3]);
 	renderingContext->quadraticCurveTo(cpx,cpy,x,y);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, arcTo, ctx, argc, argv ) {
 	if( argc < 5 ) { return NULL; }
	
 	float
 		x1 = JSValueToNumberFast(ctx, argv[0]),
 		y1 = JSValueToNumberFast(ctx, argv[1]),
 		x2 = JSValueToNumberFast(ctx, argv[2]),
 		y2 = JSValueToNumberFast(ctx, argv[3]),
 		radius = JSValueToNumberFast(ctx, argv[4]);
 	renderingContext->arcTo(x1,y1,x2,y2,radius);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, arc, ctx, argc, argv ) {
 	if( argc < 5 ) { return NULL; }
	
 	float
 		x = JSValueToNumberFast(ctx, argv[0]),
 		y = JSValueToNumberFast(ctx, argv[1]),
 		radius = JSValueToNumberFast(ctx, argv[2]),
 		startAngle = JSValueToNumberFast(ctx, argv[3]),
 		endAngle = JSValueToNumberFast(ctx, argv[4]);
 	BOOL antiClockwise = argc < 6 ? false : JSValueToBoolean(ctx, argv[5]);
 	renderingContext->arc(x,y,radius,startAngle,endAngle ,antiClockwise);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, measureText, ctx, argc, argv ) {
 	if( argc < 1 ) { return NULL; }
	
 	NSString * string = JSValueToNSString(ctx, argv[0]);
 	float stringWidth = renderingContext-> measureText(string);
	
 	JSObjectRef objRef = JSObjectMake(ctx, NULL, NULL);
 	JSStringRef stringRef = JSStringCreateWithUTF8CString("width");
 	JSObjectSetProperty(ctx, objRef, stringRef, JSValueMakeNumber(ctx, stringWidth), kJSPropertyAttributeNone, NULL);
 	JSStringRelease(stringRef);
	
 	return objRef;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, fillText, ctx, argc, argv ) {
 	if( argc < 3 ) { return NULL; }
	
 	NSString * string = JSValueToNSString(ctx, argv[0]);
 	float
 		x = JSValueToNumberFast(ctx, argv[1]),
 		y = JSValueToNumberFast(ctx, argv[2]);
	
 	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);
 	renderingContext->fillText(string,x ,y);

 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, strokeText, ctx, argc, argv ) {
 	if( argc < 3 ) { return NULL; }
	
 	NSString * string = JSValueToNSString(ctx, argv[0]);
 	float
 		x = JSValueToNumberFast(ctx, argv[1]),
 		y = JSValueToNumberFast(ctx, argv[2]);
	
 	//ejectaInstance->currentRenderingContext = renderingContext;
	ejectaInstance->setCurrentRenderingContext(renderingContext);
 	renderingContext->strokeText(string ,x ,y);
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, clip, ctx, argc, argv ) {
 	//ejectaInstance->currentRenderingContext = renderingContext;
	 ejectaInstance->setCurrentRenderingContext(renderingContext);
 	renderingContext->clip();
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, resetClip, ctx, argc, argv ) {
 	//ejectaInstance->currentRenderingContext = renderingContext;
	 ejectaInstance->setCurrentRenderingContext(renderingContext);
 	renderingContext->resetClip();
 	return NULL;
 }

 EJ_BIND_FUNCTION(EJBindingCanvas, createPattern, ctx, argc, argv) {
 	if( argc < 1 )
 	{
 		return NULL;
 	}
	EJDrawable* drawable = NULL;

	//Compare the JSObject to find out if it's an image or a canvas
	//TODO: Buffer temporary data to avoid creation at each call?
 	EJBindingImage* tempBindingImage = new EJBindingImage();
 	JSClassRef imageClass = EJApp::instance()->getJSClassForClass((EJBindingImage*)tempBindingImage);
 	if(JSValueIsObjectOfClass(ctx, argv[0], imageClass)) {
 		drawable = (EJBindingImage*)JSObjectGetPrivate((JSObjectRef)argv[0]);
 	} else {
	 	EJBindingCanvas* tempBindingCanvas = new EJBindingCanvas();
	 	JSClassRef imageClass = EJApp::instance()->getJSClassForClass((EJBindingCanvas*)tempBindingCanvas);
	 	if(JSValueIsObjectOfClass(ctx, argv[0], imageClass)) {
 			drawable = (EJBindingCanvas*)JSObjectGetPrivate((JSObjectRef)argv[0]);
	 	}
	 	delete tempBindingCanvas;
 	}
 	delete tempBindingImage;

 	if(drawable == NULL) {
 		return NULL;
 	}

	// NSObject<EJDrawable> * drawable = (NSObject<EJDrawable> *)JSObjectGetPrivate((JSObjectRef)argv[0]);
	EJTexture * image = drawable->getTexture();

 	if( !image )
 	{
 		return NULL;
 	}

 	EJCanvasPatternRepeat repeat = kEJCanvasPatternRepeat;
 	if( argc > 1 ) {
 		NSString *repeatString = JSValueToNSString(ctx, argv[1]);
 		if(repeatString->isEqual(NSStringMake("repeat-x"))) {
 			repeat = kEJCanvasPatternRepeatX;
 		}
 		else if(repeatString->isEqual(NSStringMake("repeat-y"))) {
 			repeat = kEJCanvasPatternRepeatY;
 		}
 		else if(repeatString->isEqual(NSStringMake("no-repeat"))) {
 			repeat = kEJCanvasPatternNoRepeat;
 		}
 	}
 	EJCanvasPattern *pattern = new EJCanvasPattern(image, repeat); // [[[EJCanvasPattern alloc] initWithTexture:image repeat:repeat] autorelease];


 	EJBindingCanvasPattern* tempBinding = new EJBindingCanvasPattern();
  	JSClassRef bindingClass = EJApp::instance()->getJSClassForClass((EJBindingCanvasPattern*)tempBinding);

 	JSObjectRef obj = EJBindingCanvasPattern::createJSObjectWithContext(ctx, pattern);
 	return obj;
}

 EJ_BIND_FUNCTION_NOT_IMPLEMENTED(EJBindingCanvas, createRadialGradient );
 EJ_BIND_FUNCTION_NOT_IMPLEMENTED(EJBindingCanvas, createLinearGradient );
 EJ_BIND_FUNCTION_NOT_IMPLEMENTED(EJBindingCanvas, isPointInPath );
//end

REFECTION_CLASS_IMPLEMENT(EJBindingCanvas);
