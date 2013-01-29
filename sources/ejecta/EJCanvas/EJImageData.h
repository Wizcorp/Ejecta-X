#ifndef __EJIMAGEDATA_H__
#define __EJIMAGEDATA_H__

#include "EJTexture.h"

class EJImageData: public NSObject {

public:
	EJTexture * m_texture;
	int width;
	int height;
	GLubyte * pixels;

	EJImageData(int widthp, int heightp, GLubyte * pixelsp);
	~EJImageData();

	EJTexture* texture();
};

#endif // __EJIMAGEDATA_H__