/****************************************************************************

****************************************************************************/

#ifndef __COCOA_CGAFFINETRANSFORM_H__
#define __COCOA_CGAFFINETRANSFORM_H__

#include "NSGeometry.h"
#include "support/NSPlatformMacros.h"

NS_NS_BEGIN

struct CGAffineTransform {
  float a, b, c, d;
  float tx, ty;
};

NS_DLL CGAffineTransform __CGAffineTransformMake(float a, float b, float c, float d, float tx, float ty);
#define CGAffineTransformMake __CGAffineTransformMake

NS_DLL NSPoint __NSPointApplyAffineTransform(const NSPoint& point, const CGAffineTransform& t);
#define NSPointApplyAffineTransform __NSPointApplyAffineTransform

NS_DLL NSSize __NSSizeApplyAffineTransform(const NSSize& size, const CGAffineTransform& t);
#define NSSizeApplyAffineTransform __NSSizeApplyAffineTransform

NS_DLL CGAffineTransform CGAffineTransformMakeIdentity();
NS_DLL NSRect NSRectApplyAffineTransform(const NSRect& rect, const CGAffineTransform& anAffineTransform);

NS_DLL CGAffineTransform CGAffineTransformTranslate(const CGAffineTransform& t, float tx, float ty);
NS_DLL CGAffineTransform CGAffineTransformRotate(const CGAffineTransform& aTransform, float anAngle);
NS_DLL CGAffineTransform CGAffineTransformScale(const CGAffineTransform& t, float sx, float sy);
NS_DLL CGAffineTransform CGAffineTransformConcat(const CGAffineTransform& t1, const CGAffineTransform& t2);
NS_DLL bool CGAffineTransformEqualToTransform(const CGAffineTransform& t1, const CGAffineTransform& t2);
NS_DLL bool CGAffineTransformIsIdentity(const CGAffineTransform& t);
NS_DLL CGAffineTransform CGAffineTransformInvert(const CGAffineTransform& t);

extern NS_DLL const CGAffineTransform CGAffineTransformIdentity;

NS_NS_END

#endif // __COCOA_CGAFFINETRANSFORM_H__
