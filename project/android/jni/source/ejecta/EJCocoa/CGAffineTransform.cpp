/****************************************************************************

****************************************************************************/
#include "CGAffineTransform.h"
#include <algorithm>
#include <math.h>

using namespace std;


NS_NS_BEGIN

CGAffineTransform __CGAffineTransformMake(float a, float b, float c, float d, float tx, float ty)
{
  CGAffineTransform t;
  t.a = a; t.b = b; t.c = c; t.d = d; t.tx = tx; t.ty = ty;
  return t;
}

NSPoint __NSPointApplyAffineTransform(const NSPoint& point, const CGAffineTransform& t)
{
  NSPoint p;
  p.x = (float)((double)t.a * point.x + (double)t.c * point.y + t.tx);
  p.y = (float)((double)t.b * point.x + (double)t.d * point.y + t.ty);
  return p;
}

NSSize __NSSizeApplyAffineTransform(const NSSize& size, const CGAffineTransform& t)
{
  NSSize s;
  s.width = (float)((double)t.a * size.width + (double)t.c * size.height);
  s.height = (float)((double)t.b * size.width + (double)t.d * size.height);
  return s;
}


CGAffineTransform CGAffineTransformMakeIdentity()
{
    return __CGAffineTransformMake(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

extern const CGAffineTransform CGAffineTransformIdentity = CGAffineTransformMakeIdentity();


NSRect NSRectApplyAffineTransform(const NSRect& rect, const CGAffineTransform& anAffineTransform)
{
    float top    = rect.getMinY();
    float left   = rect.getMinX();
    float right  = rect.getMaxX();
    float bottom = rect.getMaxY();
    
    NSPoint topLeft = NSPointApplyAffineTransform(NSPointMake(left, top), anAffineTransform);
    NSPoint topRight = NSPointApplyAffineTransform(NSPointMake(right, top), anAffineTransform);
    NSPoint bottomLeft = NSPointApplyAffineTransform(NSPointMake(left, bottom), anAffineTransform);
    NSPoint bottomRight = NSPointApplyAffineTransform(NSPointMake(right, bottom), anAffineTransform);

    float minX = min(min(topLeft.x, topRight.x), min(bottomLeft.x, bottomRight.x));
    float maxX = max(max(topLeft.x, topRight.x), max(bottomLeft.x, bottomRight.x));
    float minY = min(min(topLeft.y, topRight.y), min(bottomLeft.y, bottomRight.y));
    float maxY = max(max(topLeft.y, topRight.y), max(bottomLeft.y, bottomRight.y));
        
    return NSRectMake(minX, minY, (maxX - minX), (maxY - minY));
}

CGAffineTransform CGAffineTransformTranslate(const CGAffineTransform& t, float tx, float ty)
{
    return __CGAffineTransformMake(t.a, t.b, t.c, t.d, t.tx + t.a * tx + t.c * ty, t.ty + t.b * tx + t.d * ty);
}

CGAffineTransform CGAffineTransformScale(const CGAffineTransform& t, float sx, float sy)
{
    return __CGAffineTransformMake(t.a * sx, t.b * sx, t.c * sy, t.d * sy, t.tx, t.ty);
}

CGAffineTransform CGAffineTransformRotate(const CGAffineTransform& t, float anAngle)
{
    float fSin = sin(anAngle);
    float fCos = cos(anAngle);

    return __CGAffineTransformMake(    t.a * fCos + t.c * fSin,
                                    t.b * fCos + t.d * fSin,
                                    t.c * fCos - t.a * fSin,
                                    t.d * fCos - t.b * fSin,
                                    t.tx,
                                    t.ty);
}

/* Concatenate `t2' to `t1' and return the result:
     t' = t1 * t2 */
CGAffineTransform CGAffineTransformConcat(const CGAffineTransform& t1, const CGAffineTransform& t2)
{
    return __CGAffineTransformMake(    t1.a * t2.a + t1.b * t2.c, t1.a * t2.b + t1.b * t2.d, //a,b
                                    t1.c * t2.a + t1.d * t2.c, t1.c * t2.b + t1.d * t2.d, //c,d
                                    t1.tx * t2.a + t1.ty * t2.c + t2.tx,                  //tx
                                    t1.tx * t2.b + t1.ty * t2.d + t2.ty);                  //ty
}

/* Return true if `t1' and `t2' are equal, false otherwise. */
bool CGAffineTransformEqualToTransform(const CGAffineTransform& t1, const CGAffineTransform& t2)
{
    return (t1.a == t2.a && t1.b == t2.b && t1.c == t2.c && t1.d == t2.d && t1.tx == t2.tx && t1.ty == t2.ty);
}

bool CGAffineTransformIsIdentity(const CGAffineTransform& t)
{
  return CGAffineTransformEqualToTransform(CGAffineTransformIdentity, t);
}

CGAffineTransform CGAffineTransformInvert(const CGAffineTransform& t)
{
    float determinant = 1 / (t.a * t.d - t.b * t.c);

    return __CGAffineTransformMake(determinant * t.d, -determinant * t.b, -determinant * t.c, determinant * t.a,
                            determinant * (t.c * t.ty - t.d * t.tx), determinant * (t.b * t.tx - t.a * t.ty) );
}

NS_NS_END
