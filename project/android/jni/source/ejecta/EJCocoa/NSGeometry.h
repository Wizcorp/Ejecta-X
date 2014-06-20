/****************************************************************************

****************************************************************************/

#ifndef __NSGEMETRY_H__
#define __NSGEMETRY_H__

#include "support/NSPlatformMacros.h"
#include "NSObject.h"

NS_NS_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

class NS_DLL NSPoint : public NSObject
{
public:
    float x;
    float y;

public:
    NSPoint();
    NSPoint(float x, float y);
    NSPoint(const NSPoint& other);
    NSPoint& operator= (const NSPoint& other);
    void setPoint(float x, float y);
    virtual NSObject* copyWithZone(NSZone* pZone);
    bool equals(const NSPoint& target) const;

public:
    /** @deprecated use NSPoint::equals(const NSPoint&) instead, like p1.equals(p2)
     */
    NS_DEPRECATED_ATTRIBUTE static bool NSPointEqualToPoint(const NSPoint& point1, const NSPoint& point2);
};

class NS_DLL NSSize : public NSObject
{
public:
    float width;
    float height;

public:
    NSSize();
    NSSize(float width, float height);
    NSSize(const NSSize& other);
    NSSize& operator= (const NSSize& other);
    void setSize(float width, float height);
    virtual NSObject* copyWithZone(NSZone* pZone);
    bool equals(const NSSize& target) const;
    
public:
    /** @deprecated use NSSize::equals(const NSSize&) instead, like size1.equals(size2) */
    NS_DEPRECATED_ATTRIBUTE static bool NSSizeEqualToSize(const NSSize& size1, const NSSize& size2);
};

class NS_DLL NSRect : public NSObject
{
public:
    NSPoint origin;
    NSSize  size;

public:
    NSRect();    
    NSRect(float x, float y, float width, float height);
    NSRect(const NSRect& other);
    NSRect& operator= (const NSRect& other); 
    void setRect(float x, float y, float width, float height);
    virtual NSObject* copyWithZone(NSZone* pZone);
    float getMinX() const; /// return the leftmost x-value of current rect
    float getMidX() const; /// return the midpoint x-value of current rect
    float getMaxX() const; /// return the rightmost x-value of current rect
    float getMinY() const; /// return the bottommost y-value of current rect
    float getMidY() const; /// return the midpoint y-value of current rect
    float getMaxY() const; /// return the topmost y-value of current rect
    bool equals(const NSRect& rect) const;   
    bool containsPoint(const NSPoint& point) const;
    bool intersectsRect(const NSRect& rect) const;
    
public:
    /** @deprecated use NSRect::equals(const NSRect&) instead, like r1.equals(r2) */
    NS_DEPRECATED_ATTRIBUTE static bool NSRectEqualToRect(const NSRect& rect1, const NSRect& rect2);
    /** @deprecated use NSRect::containsPoint(const NSPoint&) instead, like rect.containsPoint(point) */
    NS_DEPRECATED_ATTRIBUTE static bool NSRectContainsPoint(const NSRect& rect, const NSPoint& point);
    /** @deprecated use NSRect::intersectsRect(const NSRect&) instead, like r1.intersectsRect(r2) */
    NS_DEPRECATED_ATTRIBUTE static bool NSRectIntersectsRect(const NSRect& rectA, const NSRect& rectB);
};


#define NSPointMake(x, y) NSPoint((float)(x), (float)(y))
#define NSSizeMake(width, height) NSSize((float)(width), (float)(height))
#define NSRectMake(x, y, width, height) NSRect((float)(x), (float)(y), (float)(width), (float)(height))


const NSPoint NSPointZero = NSPointMake(0,0);

/* The "zero" size -- equivalent to NSSizeMake(0, 0). */ 
const NSSize NSSizeZero = NSSizeMake(0,0);

/* The "zero" rectangle -- equivalent to NSRectMake(0, 0, 0, 0). */ 
const NSRect NSRectZero = NSRectMake(0,0,0,0);

// end of data_structure group
/// @}

NS_NS_END

#endif // __NSGEMETRY_H__
