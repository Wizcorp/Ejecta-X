#ifndef __EJ_CANVAS_CONTEXT_H__
#define __EJ_CANVAS_CONTEXT_H__

#include "EJCocoa/NSObject.h"

class EJCanvasContext : public NSObject
{
	bool needsPresenting;
public:
	short width, height;
	bool msaaEnabled;
	int msaaSamples;
	//EAGLContext *glContext;

	EJCanvasContext();
	~EJCanvasContext();

	void setScreenSize(int widthp, int heightp);

	virtual void create();
	void flushBuffers();
	virtual void prepare();
	virtual void present();
	virtual void finish();
};

#endif // __EJ_CANVAS_CONTEXT_H__

// #import <Foundation/Foundation.h>

// @class EAGLContext;
// @interface EJCanvasContext : NSObject {
// 	short width, height;
	
// 	BOOL msaaEnabled;
// 	BOOL needsPresenting;
// 	int msaaSamples;
// 	EAGLContext *glContext;
// }

// - (void)create;
// - (void)flushBuffers;
// - (void)prepare;

// @property (nonatomic) BOOL msaaEnabled;
// @property (nonatomic) int msaaSamples;
// @property (nonatomic) short width;
// @property (nonatomic) short height;
// @property (nonatomic, readonly) EAGLContext *glContext;

// @end
