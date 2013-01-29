/****************************************************************************

****************************************************************************/

#ifndef __EJ_CANVAS_CONTEXT_SCREEN_H__
#define __EJ_CANVAS_CONTEXT_SCREEN_H__

#include "EJCanvasContext.h"

typedef enum {
	kEJScalingModeNone,
	kEJScalingModeFitWidth,
	kEJScalingModeFitHeight
} EJScalingMode;

typedef struct {
	int width;
	int height;
} CGSize;

typedef struct {
	int x; 
	int y; 
	CGSize size;
} CGRect;

class EJCanvasContextScreen : EJCanvasContext {
	//EAGLView * glview;
	GLuint colorRenderbuffer;
	
	float backingStoreRatio;


public:

	bool useRetinaResolution;
	//UIDeviceOrientation orientation;
	EJScalingMode scalingMode;

	EJCanvasContextScreen();
	EJCanvasContextScreen(short widthp, short heightp);
	~EJCanvasContextScreen();
	
	virtual void create();
	virtual void prepare();
	virtual void present();
	void finish();
	virtual EJImageData* getImageData(float sx, float sy, float sw, float sh);
};


#endif // __EJ_CANVAS_CONTEXT_SCREEN_H__