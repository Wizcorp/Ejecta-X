/****************************************************************************

****************************************************************************/

#ifndef __EJ_CANVAS_CONTEXT_TEXTURE_H__
#define __EJ_CANVAS_CONTEXT_TEXTURE_H__

#include "EJCanvasContext.h"
#include "EJTexture.h"

class EJCanvasContextTexture : public EJCanvasContext {
private:
	bool msaaNeedsResolving;

	void dealloc();

	EJTexture* texture;

public:


	EJCanvasContextTexture();
	EJCanvasContextTexture(short widthp, short heightp);
	~EJCanvasContextTexture();
	
	EJTexture* getTexture();

	virtual void create();
	virtual void resizeToWidth(short newWidth, short newHeight);
	virtual void prepare();

	virtual const char* getClassName();
};

#endif // __EJ_CANVAS_CONTEXT_TEXTURE_H__