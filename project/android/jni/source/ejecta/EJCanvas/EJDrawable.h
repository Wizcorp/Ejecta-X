#ifndef __EJ_DRAWABLE_H__
#define __EJ_DRAWABLE_H__

#include "EJTexture.h"

class EJDrawable {

protected:
	EJTexture* texture;

public:
 	EJDrawable() : texture(NULL) {}
 	EJDrawable(EJTexture* tex) : texture(tex) {}
 	virtual EJTexture* getTexture() = 0;
};

#endif //__EJ_DRAWABLE_H__