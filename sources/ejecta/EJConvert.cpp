#include <stdlib.h>
#include <stdint.h>
#include "EJConvert.h"

NSString * JSValueToNSString( JSContextRef ctx, JSValueRef v ) {
	JSStringRef jsString = JSValueToStringCopy( ctx, v, NULL );
	if( !jsString ) return NULL;
	
      size_t stringUTF8Size = JSStringGetMaximumUTF8CStringSize(jsString);
      char* c_string = (char*)malloc(stringUTF8Size);
      JSStringGetUTF8CString(jsString, c_string, stringUTF8Size);
	NSString * string = new NSString(c_string);
	string->autorelease();
      free(c_string);
	JSStringRelease( jsString );
	
	return string;
}

JSValueRef NSStringToJSValue( JSContextRef ctx, NSString * string ) {
	JSStringRef jstr = JSStringCreateWithUTF8CString(string->getCString());
	JSValueRef ret = JSValueMakeString(ctx, jstr);
	JSStringRelease(jstr);
	return ret;
}

JSValueRef NSStringToJSValueProtect( JSContextRef ctx, NSString * string ) {
	JSValueRef ret = NSStringToJSValue( ctx, string );
	JSValueProtect(ctx, ret);
	return ret;
}

double JSValueToNumberFast( JSContextRef ctx, JSValueRef v ) {
      unsigned char * bytes = ((unsigned char *) v) + 8;
      unsigned char * tagBytes = ((unsigned char *) v) + 12;
      int32_t unionTag = *((int32_t*)tagBytes);
      if( unionTag < 0xfffffff8 ) {
            return *((double *) bytes);
      }else {
            return *((int32_t *) bytes);
      }         
}

//static const EJColorRGBA ColorNames[] = {
//      {0xff000000}, // invaid
//      {0xfffff8f0}, // aliceblue
//      {0xffd7ebfa}, // antiquewhite
//      {0xffffff00}, // aqua
//      {0xffd4ff7f}, // aquamarine
//      {0xfffffff0}, // azure
//      {0xffdcf5f5}, // beige
//      {0xffc4e4ff}, // bisque
//      {0xff000000}, // black
//      {0xffcdebff}, // blanchedalmond
//      {0xffff0000}, // blue
//      {0xffe22b8a}, // blueviolet
//      {0xff2a2aa5}, // brown
//      {0xff87b8de}, // burlywood
//      {0xffa09e5f}, // cadetblue
//      {0xff00ff7f}, // chartreuse
//      {0xff1e69d2}, // chocolate
//      {0xff507fff}, // coral
//      {0xffed9564}, // cornflowerblue
//      {0xffdcf8ff}, // cornsilk
//      {0xff3c14dc}, // crimson
//      {0xffffff00}, // cyan
//      {0xff8b0000}, // darkblue
//      {0xff8b8b00}, // darkcyan
//      {0xff0b86b8}, // darkgoldenrod
//      {0xffa9a9a9}, // darkgray
//      {0xff006400}, // darkgreen
//      {0xff6bb7bd}, // darkkhaki
//      {0xff8b008b}, // darkmagenta
//      {0xff2f6b55}, // darkolivegreen
//      {0xff008cff}, // darkorange
//      {0xffcc3299}, // darkorchid
//      {0xff00008b}, // darkred
//      {0xff7a96e9}, // darksalmon
//      {0xff8fbc8f}, // darkseagreen
//      {0xff8b3d48}, // darkslateblue
//      {0xff4f4f2f}, // darkslategray
//      {0xffd1ce00}, // darkturquoise
//      {0xffd30094}, // darkviolet
//      {0xff9314ff}, // deeppink
//      {0xffffbf00}, // deepskyblue
//      {0xff696969}, // dimgray
//      {0xffff901e}, // dodgerblue
//      {0xff2222b2}, // firebrick
//      {0xfff0faff}, // floralwhite
//      {0xff228b22}, // forestgreen
//      {0xffff00ff}, // fuchsia
//      {0xffdcdcdc}, // gainsboro
//      {0xfffff8f8}, // ghostwhite
//      {0xff00d7ff}, // gold
//      {0xff20a5da}, // goldenrod
//      {0xff808080}, // gray
//      {0xff008000}, // green
//      {0xff2fffad}, // greenyellow
//      {0xfff0fff0}, // honeydew
//      {0xffb469ff}, // hotpink
//      {0xff5c5ccd}, // indianred
//      {0xff82004b}, // indigo
//      {0xfff0ffff}, // ivory
//      {0xff8ce6f0}, // khaki
//      {0xfffae6e6}, // lavender
//      {0xfff5f0ff}, // lavenderblush
//      {0xff00fc7c}, // lawngreen
//      {0xffcdfaff}, // lemonchiffon
//      {0xffe6d8ad}, // lightblue
//      {0xff8080f0}, // lightcoral
//      {0xffffffe0}, // lightcyan
//      {0xffd2fafa}, // lightgoldenrodyellow
//      {0xffd3d3d3}, // lightgray
//      {0xff90ee90}, // lightgreen
//      {0xffc1b6ff}, // lightpink
//      {0xff7aa0ff}, // lightsalmon
//      {0xffaab220}, // lightseagreen
//      {0xffface87}, // lightskyblue
//      {0xff998877}, // lightslategray
//      {0xffdec4b0}, // lightsteelblue
//      {0xffe0ffff}, // lightyellow
//      {0xff00ff00}, // lime
//      {0xff32cd32}, // limegreen
//      {0xffe6f0fa}, // linen
//      {0xffff00ff}, // magenta
//      {0xff000080}, // maroon
//      {0xffaacd66}, // mediumaquamarine
//      {0xffcd0000}, // mediumblue
//      {0xffd355ba}, // mediumorchid
//      {0xffd87093}, // mediumpurple
//      {0xff71b33c}, // mediumseagreen
//      {0xffee687b}, // mediumslateblue
//      {0xff9afa00}, // mediumspringgreen
//      {0xffccd148}, // mediumturquoise
//      {0xff8515c7}, // mediumvioletred
//      {0xff701919}, // midnightblue
//      {0xfffafff5}, // mintcream
//      {0xffe1e4ff}, // mistyrose
//      {0xffb5e4ff}, // moccasin
//      {0xffaddeff}, // navajowhite
//      {0xff800000}, // navy
//      {0xffe6f5fd}, // oldlace
//      {0xff008080}, // olive
//      {0xff238e6b}, // olivedrab
//      {0xff00a5ff}, // orange
//      {0xff0045ff}, // orangered
//      {0xffd670da}, // orchid
//      {0xffaae8ee}, // palegoldenrod
//      {0xff98fb98}, // palegreen
//      {0xffeeeeaf}, // paleturquoise
//      {0xff9370d8}, // palevioletred
//      {0xffd5efff}, // papayawhip
//      {0xffb9daff}, // peachpuff
//      {0xff3f85cd}, // peru
//      {0xffcbc0ff}, // pink
//      {0xffdda0dd}, // plum
//      {0xffe6e0b0}, // powderblue
//      {0xff800080}, // purple
//      {0xff0000ff}, // red
//      {0xff8f8fbc}, // rosybrown
//      {0xffe16941}, // royalblue
//      {0xff13458b}, // saddlebrown
//      {0xff7280fa}, // salmon
//      {0xff60a4f4}, // sandybrown
//      {0xff578b2e}, // seagreen
//      {0xffeef5ff}, // seashell
//      {0xff2d52a0}, // sienna
//      {0xffc0c0c0}, // silver
//      {0xffebce87}, // skyblue
//      {0xffcd5a6a}, // slateblue
//      {0xff908070}, // slategray
//      {0xfffafaff}, // snow
//      {0xff7fff00}, // springgreen
//      {0xffb48246}, // steelblue
//      {0xff8cb4d2}, // tan
//      {0xff808000}, // teal
//      {0xffd8bfd8}, // thistle
//      {0xff4763ff}, // tomato
//      {0xffd0e040}, // turquoise
//      {0xffee82ee}, // violet
//      {0xffb3def5}, // wheat
//      {0xffffffff}, // white
//      {0xfff5f5f5}, // whitesmoke
//      {0xff00ffff}, // yellow
//      {0xff32cd9a}  // yellowgreen
//};

// This is a bit wild. ColorHashForString returns a hash of the input string in the range
// of [0 -- 497] (the smallest range I could find with such a simple hash function). This
// hash can then be used as an index into the ColorHashesToColorNames table, which in turn
// points to the actual color in the ColorNames array. It's totally over the top, considering
// that nobody actually uses those names anymore.
// Looking up color names from an NSDictionary takes 4 times as long, but it's still next
// to nothing (~0.02ms). At least I learned a few things about perfect hashes today :)

static const unsigned char ColorHashesToColorNames[498] = {
	0,0,0,0,0,0,0,110,0,0,139,59,0,18,0,0,63,0,0,55,0,0,0,0,105,6,0,115,12,0,3,0,14,133,131,0,0,114,0,17,95,0,0,0,0,0,0,21,0,0,
	0,22,94,0,0,119,0,0,0,0,0,111,0,0,0,0,8,0,0,65,0,0,0,0,0,0,0,0,32,0,0,0,0,0,0,0,47,0,0,0,0,7,93,0,0,0,0,0,31,89,15,0,0,10,0,
	0,0,0,85,0,0,0,108,0,0,0,0,56,0,0,0,0,138,0,0,0,0,0,0,5,58,0,0,0,0,0,0,0,0,0,0,109,0,0,4,60,38,36,0,0,0,50,0,0,0,0,0,0,0,78,
	0,0,11,99,30,0,0,0,0,0,80,0,0,125,0,0,0,0,0,91,0,0,0,0,0,0,41,0,0,0,0,98,0,0,127,0,66,0,1,100,0,73,0,0,0,0,83,0,123,0,46,0,
	0,0,0,0,0,84,0,0,0,0,51,0,0,0,87,0,0,0,27,0,134,0,0,0,0,0,0,19,74,23,0,9,57,49,0,0,0,0,16,39,126,0,20,0,25,0,72,82,0,0,132,
	0,0,0,0,136,0,0,0,0,0,0,0,0,0,0,0,0,28,0,0,0,0,0,64,0,24,0,0,0,0,0,0,68,0,81,96,0,0,0,0,0,0,135,0,67,124,77,42,0,0,0,0,0,33,
	0,0,76,43,0,128,0,107,0,0,0,0,0,0,0,79,0,45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,92,0,0,71,0,70,121,0,0,0,0,106,0,0,137,122,
	0,0,62,104,0,0,29,101,0,0,0,0,0,0,140,0,54,0,90,69,0,0,0,0,0,0,40,0,130,0,102,2,0,0,0,0,0,0,0,0,0,113,0,44,0,0,0,61,0,0,0,0,
	0,0,0,0,0,0,0,35,13,0,0,52,75,0,0,0,53,0,0,0,88,97,86,0,0,0,0,0,0,0,0,117,0,0,0,0,129,0,0,0,0,0,103,0,26,0,0,0,48,0,0,0,0,0,
	118,116,0,0,37,0,0,0,0,0,0,0,0,0,0,0,112,120,0,34,0,0,0
};

static inline unsigned int ColorHashForString( const JSChar * s, int length ) {
	unsigned int h = 0;
	for( int i = 0; i < length; i++ ) {
		h = (h << 6) ^ (h >> 26) ^ (tolower(s[i]) * 1978555465);
	}
	return (h % 498);
};

//EJColorRGBA JSValueToColorRGBA(JSContextRef ctx, JSValueRef value) {
//	EJColorRGBA c = {0xff000000};
//	if( !JSValueIsString(ctx, value) ) { return c; }
//	
//	JSStringRef jsString = JSValueToStringCopy( ctx, value, NULL );
//	int length = JSStringGetLength( jsString );
//	
//	if( length < 3 ) { return c; }
//	
//	const JSChar * jsc = JSStringGetCharactersPtr(jsString);
//	char str[] = "ffffff";
//	
//	// #f0f format
//	if( jsc[0] == '#' && length == 4 ) {
//		str[0] = str[1] = jsc[3];
//		str[2] = str[3] = jsc[2];
//		str[4] = str[5] = jsc[1];
//		c.hex = 0xff000000 | strtol( str, NULL, 16 );
//	}
//	
//	// #ff00ff format
//	else if( jsc[0] == '#' && length == 7 ) {
//		str[0] = jsc[5];
//		str[1] = jsc[6];
//		str[2] = jsc[3];
//		str[3] = jsc[4];
//		str[4] = jsc[1];
//		str[5] = jsc[2];
//		c.hex = 0xff000000 | strtol( str, NULL, 16 );
//	}
//	
//	// assume rgb(255,0,255) or rgba(255,0,255,0.5) format
//	else if( jsc[0] == 'r' && jsc[1] == 'g' ) {
//		int component = 0;
//		for( int i = 4; i < length-1 && component < 4; i++ ) {
//			if( component == 3 ) {
//				// If we have an alpha component, copy the rest of the wide
//				// string into a char array and use atof() to parse it.
//				char alpha[8] = { 0,0,0,0, 0,0,0,0 };
//				for( int j = 0; i + j < length-1 && j < 7; j++ ) {
//					alpha[j] = jsc[i+j];
//				}
//				c.components[component] = atof(alpha) * 255.0f;
//				component++;
//			}
//			else if( isdigit(jsc[i]) ) {
//				c.components[component] = c.components[component] * 10 + (jsc[i] - '0'); 
//			}
//			else if( jsc[i] == ',' || jsc[i] == ')' ) {
//				component++;
//			}
//		}
//	}
//	
//	// try color name
//	else {
//		unsigned int hash = ColorHashForString( jsc, length );
//		c = ColorNames[ColorHashesToColorNames[hash]];
//	}
//	
//	JSStringRelease(jsString);
//	return c;
//}
//
//JSValueRef ColorRGBAToJSValue( JSContextRef ctx, EJColorRGBA c ) {
//	static char buffer[32];
//	sprintf(buffer, "rgba(%d,%d,%d,%.3f)", c.rgba.r, c.rgba.g, c.rgba.b, (float)c.rgba.a/255.0f );
//	
//	JSStringRef src = JSStringCreateWithUTF8CString( buffer );
//	JSValueRef ret = JSValueMakeString(ctx, src);
//	JSStringRelease(src);
//	return ret;
//}

JSObjectRef ByteArrayToJSObject( JSContextRef ctx, unsigned char * bytes, int count ) {
	// This creates a JSON string from a byte array and then uses the JSC APIs
	// JSValueMakeFromJSONString function to convert it into a JSObject that
	// is an array. It's faster than repeatedly calling JSValueMakeNumber and
	// JSValueMakeArray.

	// This sucks. Where's the JSC API for ByteArrays?
	
	
	if( count < 1 ) {
		return JSObjectMakeArray(ctx, 0, NULL, NULL);
	}
	
	// Build the JSON string from the byte array
	int bufferLength = count * 4 + 3; // Max 4 chars per element + "[]\0"
	char * jsonBuffer = (char *)malloc( bufferLength );
		
	jsonBuffer[0] = '[';
	int pos = 0;
	for( int i = 0; i < count; i++ ) {
		unsigned char n = *(bytes + i);
		if( n > 99 ) { jsonBuffer[++pos] = (n / 100) + '0'; }
		if( n > 9 ) { jsonBuffer[++pos] = ((n % 100)/10) + '0'; }
		jsonBuffer[++pos] = (n % 10) + '0';
		jsonBuffer[++pos] = ',';
	}
	
	jsonBuffer[pos] = ']'; // overwrite last comma
	jsonBuffer[++pos] = '\0';
	
	// Convert the json string to an object
	JSStringRef jss = JSStringCreateWithUTF8CString(jsonBuffer);
	JSObjectRef array = (JSObjectRef)JSValueMakeFromJSONString(ctx, jss);
	
	free(jsonBuffer);
	JSStringRelease(jss);
	
	return array;
}

void JSObjectToByteArray( JSContextRef ctx, JSObjectRef array, unsigned char * bytes, int count ) {
	// Converting a JSArray to byte buffer seems to be faster without the JSON intermediate
	for( int i = 0; i < count; i++ ) {
		bytes[i] = JSValueToNumberFast(ctx, JSObjectGetPropertyAtIndex(ctx, array, i, NULL));
	}
}

void JSValueUnprotectSafe( JSContextRef ctx, JSValueRef v ) {
	if( ctx && v ) {
		JSValueUnprotect(ctx, v);
	}
}

// #import "EJConvert.h"

// NSString *JSValueToNSString( JSContextRef ctx, JSValueRef v ) {
// 	JSStringRef jsString = JSValueToStringCopy( ctx, v, NULL );
// 	if( !jsString ) return nil;
	
// 	NSString *string = (NSString *)JSStringCopyCFString( kCFAllocatorDefault, jsString );
// 	[string autorelease];
// 	JSStringRelease( jsString );
	
// 	return string;
// }

// JSValueRef NSStringToJSValue( JSContextRef ctx, NSString *string ) {
// 	JSStringRef jstr = JSStringCreateWithCFString((CFStringRef)string);
// 	JSValueRef ret = JSValueMakeString(ctx, jstr);
// 	JSStringRelease(jstr);
// 	return ret;
// }

// double JSValueToNumberFast( JSContextRef ctx, JSValueRef v ) {
// 	// This struct represents the memory layout of a C++ JSValue instance
// 	// See JSC/runtime/JSValue.h for an explanation of the tagging
// 	struct {
// 		unsigned char cppClassData[8];
// 		union {
// 			double asDouble;
// 			struct { int32_t asInt; int32_t tag; } asBits;
// 		} payload;
// 	} *decoded = (void *)v;
	
// 	return decoded->payload.asBits.tag < 0xfffffff9
// 		? decoded->payload.asDouble
// 		: decoded->payload.asBits.asInt;
// }

// void JSValueUnprotectSafe( JSContextRef ctx, JSValueRef v ) {
// 	if( ctx && v ) {
// 		JSValueUnprotect(ctx, v);
// 	}
// }

