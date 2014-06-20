#ifndef __EJPATH_H__
#define __EJPATH_H__

#include <math.h>
#include <vector>
#include "EJCanvas2DTypes.h"
#include "EJCanvasContext.h"
#include "EJCocoa/support/NSPlatformMacros.h"

#define EJ_PATH_RECURSION_LIMIT 8
#define EJ_PATH_DISTANCE_EPSILON 1.0f
#define EJ_PATH_COLLINEARITY_EPSILON FLT_EPSILON
#define EJ_PATH_STEPS_FOR_CIRCLE 48.0f

typedef enum {
	kEJPathPolygonTargetColor,
	kEJPathPolygonTargetDepth
} EJPathPolygonTarget;


// We're using the C++ std::vector here to store our points. Boxing and unboxing
// so many EJVectors to NSValue types seemed wasteful.
typedef std::vector<EJVector2> points_t;
typedef struct {
	points_t points;
	bool isClosed;
} subpath_t;
typedef std::vector<subpath_t> path_t;

class EJCanvasContext;

class EJPath: public NSObject {
private:
	subpath_t currentPath;
	path_t paths;

	EJVector2 currentPos, startPos, lastPushed;
	EJVector2 minPos, maxPos;
	int longestSubpath;

	GLubyte stencilMask;

	float distanceTolerance;

	EJPath* copyWithZone(NSZone * zone);
	void drawArcToContext(EJCanvasContext * context, EJVector2 point,
			EJVector2 p1, EJVector2 p2, EJColorRGBA color);

public:
	CGAffineTransform transform;

	EJPath();

	void push(EJVector2 v);
	void reset();
	void close();
	void endSubPath();
	void moveTo(float x, float y);
	void lineTo(float x, float y);
	void bezierCurveTo(float cpx1, float cpy1, float cpx2, float cpy2,
			float x, float y, float scale);
	void recursiveBezier(float x1, float y1, float x2, float y2, float x3,
			float y3, float x4, float y4, int level);
	void quadraticCurveTo(float cpx, float cpy, float x, float y,
			float scale);
	void recursiveQuadratic(float x1, float y1, float x2, float y2, float x3,
			float y3, int level);
	void arcTo(float x1, float y1, float x2, float y2, float radius);
	void arc(float x, float y, float radius, float startAngle, float endAngle,
			bool antiClockwise);

	void drawPolygonsToContext(EJCanvasContext * context,
			EJPathPolygonTarget target);
	void drawLinesToContext(EJCanvasContext * context);

};


#endif // __EJPATH_H__
