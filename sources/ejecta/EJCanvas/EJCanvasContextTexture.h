/****************************************************************************

****************************************************************************/

#ifndef __EJ_CANVAS_CONTEXT_TEXTURE_H__
#define __EJ_CANVAS_CONTEXT_TEXTURE_H__

#include "EJCanvasContext.h"
#include "EJTexture.h"

class EJCanvasContextTexture : EJCanvasContext {
private:
	bool msaaNeedsResolving;

	void dealloc();
	EJTexture* texture();

public:


	EJCanvasContextTexture();
	EJCanvasContextTexture(short widthp, short heightp);
	~EJCanvasContextTexture();
	
	EJTexture* m_texture;

	virtual void create();
	virtual void prepare();

	virtual const char* getClassName();
};

#endif // __EJ_CANVAS_CONTEXT_TEXTURE_H__