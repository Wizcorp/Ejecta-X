/****************************************************************************

****************************************************************************/

#include "NSGeometry.h"
#include "support/nsMacros.h"

// implementation of NSPoint
NS_NS_BEGIN

NSPoint::NSPoint(void)
{
    setPoint(0.0f, 0.0f);
}

NSPoint::NSPoint(float x, float y)
{
    setPoint(x, y);
}

NSPoint::NSPoint(const NSPoint& other)
{
    setPoint(other.x, other.y);
}

NSPoint& NSPoint::operator= (const NSPoint& other)
{
    setPoint(other.x, other.y);
    return *this;
}

void NSPoint::setPoint(float x, float y)
{
    this->x = x;
    this->y = y;
}

NSObject* NSPoint::copyWithZone(NSZone* pZone)
{
    NSPoint* pRet = new NSPoint();
    pRet->setPoint(this->x, this->y);
    return pRet;
}

bool NSPoint::equals(const NSPoint& target) const
{
    return ((x == target.x) && (y == target.y));
}

bool NSPoint::NSPointEqualToPoint(const NSPoint& point1, const NSPoint& point2)
{
    return point1.equals(point2);
}

// implementation of NSSize

NSSize::NSSize(void)
{
    setSize(0.0f, 0.0f);
}

NSSize::NSSize(float width, float height)
{
    setSize(width, height);
}

NSSize::NSSize(const NSSize& other)
{
    setSize(other.width, other.height);
}

NSSize& NSSize::operator= (const NSSize& other)
{
    setSize(other.width, other.height);
    return *this;
}

void NSSize::setSize(float width, float height)
{
    this->width = width;
    this->height = height;
}

NSObject* NSSize::copyWithZone(NSZone* pZone)
{
    NSSize* pRet = new NSSize();
    pRet->setSize(this->width, this->width);
    return pRet;
}

bool NSSize::equals(const NSSize& target) const
{
    return ((width == target.width) && (height == target.height));
}


bool NSSize::NSSizeEqualToSize(const NSSize& size1, const NSSize& size2)
{
    return size1.equals(size2);
}

// implementation of NSRect

NSRect::NSRect(void)
{
    setRect(0.0f, 0.0f, 0.0f, 0.0f);
}

NSRect::NSRect(float x, float y, float width, float height)
{
    setRect(x, y, width, height);
}

NSRect::NSRect(const NSRect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
}

NSRect& NSRect::operator= (const NSRect& other)
{
    setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
    return *this;
}

void NSRect::setRect(float x, float y, float width, float height)
{
    // Only support that, the width and height > 0
    NSAssert(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0.");

    origin.x = x;
    origin.y = y;

    size.width = width;
    size.height = height;
}

NSObject* NSRect::copyWithZone(NSZone* pZone)
{
    NSRect* pRet = new NSRect();
    pRet->setRect(this->origin.x, this->origin.y, this->size.width, this->size.height);
    return pRet;
}

bool NSRect::equals(const NSRect& rect) const
{
    return (origin.equals(rect.origin) && 
            size.equals(rect.size));
}

float NSRect::getMaxX() const
{
    return (float)(origin.x + size.width);
}

float NSRect::getMidX() const
{
    return (float)(origin.x + size.width / 2.0);
}

float NSRect::getMinX() const
{
    return origin.x;
}

float NSRect::getMaxY() const
{
    return origin.y + size.height;
}

float NSRect::getMidY() const
{
    return (float)(origin.y + size.height / 2.0);
}

float NSRect::getMinY() const
{
    return origin.y;
}

bool NSRect::containsPoint(const NSPoint& point) const
{
    bool bRet = false;

    if (point.x >= getMinX() && point.x <= getMaxX()
        && point.y >= getMinY() && point.y <= getMaxY())
    {
        bRet = true;
    }

    return bRet;
}

bool NSRect::intersectsRect(const NSRect& rect) const
{
    return !(     getMaxX() < rect.getMinX() ||
             rect.getMaxX() <      getMinX() ||
                  getMaxY() < rect.getMinY() ||
             rect.getMaxY() <      getMinY());
}

bool NSRect::NSRectEqualToRect(const NSRect& rect1, const NSRect& rect2)
{
    return rect1.equals(rect2);
}

bool NSRect::NSRectContainsPoint(const NSRect& rect, const NSPoint& point)
{
    return rect.containsPoint(point);
}

bool NSRect::NSRectIntersectsRect(const NSRect& rectA, const NSRect& rectB)
{
    /*
    return !(NSRectGetMaxX(rectA) < NSRectGetMinX(rectB)||
            NSRectGetMaxX(rectB) < NSRectGetMinX(rectA)||
            NSRectGetMaxY(rectA) < NSRectGetMinY(rectB)||
            NSRectGetMaxY(rectB) < NSRectGetMinY(rectA));
     */
    return rectA.intersectsRect(rectB);
}

NS_NS_END
