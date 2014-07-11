#include "EJCanvasPattern.h"

EJCanvasPattern::EJCanvasPattern()
{
}

EJCanvasPattern::EJCanvasPattern(EJTexture* pTexture, EJCanvasPatternRepeat eRepeat)
{
	initWithTexture(pTexture, eRepeat);
}

EJCanvasPattern::~EJCanvasPattern()
{
	if(texture != NULL)
	{
		texture->release();
	}
}
	
EJCanvasPattern* EJCanvasPattern::initWithTexture(EJTexture* pTexture, EJCanvasPatternRepeat eRepeat)
{
	texture = pTexture;
	texture->retain();
	repeat = eRepeat;
	
	return this;
}

