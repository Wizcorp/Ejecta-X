#ifndef __EJFONT_H__
#define __EJFONT_H__

#include "EJTexture.h"
#include "../EJCocoa/NSArray.h"
#include "../EJCocoa/NSInteger.h"
#include "../EJCocoa/NSString.h"

#define EJ_FONT_TEXTURE_SIZE 1024

class EJCanvasContext;

class EJFont : public NSObject {
	// Glyph information
	//__gnu_cxx::hash_map<int, GlyphInfo> glyphInfoMap;
	float txLineX, txLineY, txLineH;
	
	void * font_info;
	unsigned long font_index;
	size_t font_size;
	//GlyphLayout * layoutBuffer;
	
	// Font preferences
	float pointSize, ascent, ascentDelta, descent, leading, lineHeight, contentScale;
	BOOL fill;
	
	// Font references
	//CTFontRef ctMainFont;
	//CGFontRef cgMainFont;
	
	// Core text variables for line layout
	//CGGlyph * glyphsBuffer;
	//CGPoint * positionsBuffer;
public:

	EJTexture * texture;
	unsigned int width, height;

	EJFont();
	EJFont(NSString* font, NSInteger size, BOOL fill, float contentScale);
	~EJFont();

	void drawString(NSString* string, EJCanvasContext* context, float x, float y);
	float measureString(NSString* string);

	/* data */
};

#endif // __EJFONT_H__