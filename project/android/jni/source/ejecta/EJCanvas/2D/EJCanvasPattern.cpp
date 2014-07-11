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
	// TODO ttt Need to dupe the texture here to duplicate functionality on iOS Ejecta
	texture = pTexture;
	texture->retain();
	repeat = eRepeat;
	
	return this;
}

/*	
@implementation EJCanvasPattern

@synthesize texture;
@synthesize repeat;

- (id)initWithTexture:(EJTexture *)texturep repeat:(EJCanvasPatternRepeat)repeatp {
	if( self = [super init] ) {
		texture = texturep.copy;
		repeat = repeatp;
	}
	return self;
}

- (void)dealloc {
	[texture release];
	[super dealloc];
}

@end
*/
