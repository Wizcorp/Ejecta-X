#ifndef __EJ_CANVAS_PATTERN_H__
#define __EJ_CANVAS_PATTERN_H__

#include "EJTexture.h"
#include "EJCanvasContext.h"

typedef enum {
	kEJCanvasPatternNoRepeat = 0,
	kEJCanvasPatternRepeatX = 1,
	kEJCanvasPatternRepeatY = 2,
	kEJCanvasPatternRepeat = 1 | 2
} EJCanvasPatternRepeat;

class EJCanvasPattern : public EJFillable {
private:
	EJTexture *texture;
	EJCanvasPatternRepeat repeat;
public:
	EJCanvasPattern();
	EJCanvasPattern(EJTexture* pTexture, EJCanvasPatternRepeat eRepeat);
	~EJCanvasPattern();
	EJCanvasPattern* initWithTexture(EJTexture* pTexture, EJCanvasPatternRepeat eRepeat);
	EJTexture* GetTexture() { return texture; }
	EJCanvasPatternRepeat GetRepeat() { return repeat; }
};

#endif
