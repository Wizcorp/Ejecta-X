/****************************************************************************

****************************************************************************/

#ifndef __PLATFOMR_NSNS_H__
#define __PLATFOMR_NSNS_H__

#include "NSGeometry.h"

NS_NS_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

/**
@brief Returns a Core Graphics rectangle structure corresponding to the data in a given string.
@param pszContent   A string object whose contents are of the form "{{x,y},{w, h}}",
                    where x is the x coordinate, y is the y coordinate, w is the width, and h is the height.
                    These components can represent integer or float values.
                    An example of a valid string is "{{3,2},{4,5}}".
                    The string is not localized, so items are always separated with a comma.
@return A Core Graphics structure that represents a rectangle.
        If the string is not well-formed, the function returns NSRectZero.
*/
NSRect NS_DLL NSRectFromString(const char* pszContent);

/**
@brief Returns a Core Graphics point structure corresponding to the data in a given string.
@param pszContent   A string object whose contents are of the form "{x,y}",
                    where x is the x coordinate and y is the y coordinate.
                    The x and y values can represent integer or float values.
                    An example of a valid string is "{3.0,2.5}".
                    The string is not localized, so items are always separated with a comma.
@return A Core Graphics structure that represents a point.
        If the string is not well-formed, the function returns NSPointZero.
*/
NSPoint NS_DLL NSPointFromString(const char* pszContent);

/**
@brief Returns a Core Graphics size structure corresponding to the data in a given string.
@param pszContent   A string object whose contents are of the form "{w, h}",
                    where w is the width and h is the height.
                    The w and h values can be integer or float values.
                    An example of a valid string is "{3.0,2.5}".
                    The string is not localized, so items are always separated with a comma.
@return A Core Graphics structure that represents a size.
        If the string is not well-formed, the function returns NSSizeZero.
*/
NSSize NS_DLL NSSizeFromString(const char* pszContent);

// end of data_structure group
/// @}

NS_NS_END

#endif // __PLATFOMR_NSNS_H__


