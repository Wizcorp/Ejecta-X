/****************************************************************************

****************************************************************************/

#ifndef __NSMACROS_H__
#define __NSMACROS_H__

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

//#include "platform/NSCommon.h"
//#include "NSStdC.h"

#ifndef NSAssert
#ifdef _WINDOWS
#define NSAssert(cond, msg)         NS_ASSERT_S(cond)
#else
#define NSAssert(cond, msg)         NS_ASSERT(cond)
#endif
#endif  // NSAssert

#ifndef MAX
#define MAX(x, y)         x < y ? y : x
#endif  // MAX

#ifndef MIN
#define MIN(x, y)         x < y ? x : y
#endif  // MIN

#ifdef _WINDOWS
#ifndef INFINITY
#define INFINITY     0x221E
#endif
#else
#ifndef BOOL
#define BOOL         bool
#endif  // BOOL
#endif

#include "nsConfig.h"

/** @def NS_SWAP
simple macro that swaps 2 variables
*/
#define NS_SWAP(x, y, type)    \
{    type temp = (x);        \
    x = y; y = temp;        \
}


/** @def NSRANDOM_MINUS1_1
 returns a random float between -1 and 1
 */
#define NSRANDOM_MINUS1_1() ((2.0f*((float)rand()/RAND_MAX))-1.0f)

/** @def NSRANDOM_0_1
 returns a random float between 0 and 1
 */
#define NSRANDOM_0_1() ((float)rand()/RAND_MAX)

/** @def NS_DEGREES_TO_RADIANS
 converts degrees to radians
 */
#define NS_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180

/** @def NS_RADIANS_TO_DEGREES
 converts radians to degrees
 */
#define NS_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

#define kNSRepeatForever UINT_MAX -1

/** @def NS_BLEND_SRC
default gl blend src function. Compatible with premultiplied alpha images.
*/
#define NS_BLEND_SRC GL_ONE
#define NS_BLEND_DST GL_ONE_MINUS_SRC_ALPHA


/** @def NS_NODE_DRAW_SETUP
 Helpful macro that setups the GL server state, the correct GL program and sets the Model View Projection matrix
 @since v2.0
 */
#define NS_NODE_DRAW_SETUP() \
do { \
    nsGLEnable( m_glServerState ); \
    NSAssert(getShaderProgram(), "No shader program set for this node"); \
    { \
        getShaderProgram()->use(); \
        getShaderProgram()->setUniformForModelViewProjectionMatrix(); \
    } \
} while(0)


 /** @def NS_DIRECTOR_END
  Stops and removes the director from memory.
  Removes the NSGLView from its parent

  @since v0.99.4
  */
#define NS_DIRECTOR_END()                                        \
do {                                                            \
    NSDirector *__director = NSDirector::sharedDirector();        \
    __director->end();                                            \
} while(0)

/** @def NS_CONTENT_SCALE_FACTOR
On Mac it returns 1;
On iPhone it returns 2 if RetinaDisplay is On. Otherwise it returns 1
*/
#define NS_CONTENT_SCALE_FACTOR() NSDirector::sharedDirector()->getContentScaleFactor()

/****************************/
/** RETINA DISPLAY ENABLED **/
/****************************/

/** @def NS_RECT_PIXELS_TO_POINTS
 Converts a rect in pixels to points
 */
#define NS_RECT_PIXELS_TO_POINTS(__rect_in_pixels__)                                                                        \
    NSRectMake( (__rect_in_pixels__).origin.x / NS_CONTENT_SCALE_FACTOR(), (__rect_in_pixels__).origin.y / NS_CONTENT_SCALE_FACTOR(),    \
            (__rect_in_pixels__).size.width / NS_CONTENT_SCALE_FACTOR(), (__rect_in_pixels__).size.height / NS_CONTENT_SCALE_FACTOR() )

/** @def NS_RECT_POINTS_TO_PIXELS
 Converts a rect in points to pixels
 */
#define NS_RECT_POINTS_TO_PIXELS(__rect_in_points_points__)                                                                        \
    NSRectMake( (__rect_in_points_points__).origin.x * NS_CONTENT_SCALE_FACTOR(), (__rect_in_points_points__).origin.y * NS_CONTENT_SCALE_FACTOR(),    \
            (__rect_in_points_points__).size.width * NS_CONTENT_SCALE_FACTOR(), (__rect_in_points_points__).size.height * NS_CONTENT_SCALE_FACTOR() )

/** @def NS_POINT_PIXELS_TO_POINTS
 Converts a rect in pixels to points
 */
#define NS_POINT_PIXELS_TO_POINTS(__pixels__)                                                                        \
NSPointMake( (__pixels__).x / NS_CONTENT_SCALE_FACTOR(), (__pixels__).y / NS_CONTENT_SCALE_FACTOR())

/** @def NS_POINT_POINTS_TO_PIXELS
 Converts a rect in points to pixels
 */
#define NS_POINT_POINTS_TO_PIXELS(__points__)                                                                        \
NSPointMake( (__points__).x * NS_CONTENT_SCALE_FACTOR(), (__points__).y * NS_CONTENT_SCALE_FACTOR())

/** @def NS_POINT_PIXELS_TO_POINTS
 Converts a rect in pixels to points
 */
#define NS_SIZE_PIXELS_TO_POINTS(__size_in_pixels__)                                                                        \
NSSizeMake( (__size_in_pixels__).width / NS_CONTENT_SCALE_FACTOR(), (__size_in_pixels__).height / NS_CONTENT_SCALE_FACTOR())

/** @def NS_POINT_POINTS_TO_PIXELS
 Converts a rect in points to pixels
 */
#define NS_SIZE_POINTS_TO_PIXELS(__size_in_points__)                                                                        \
NSSizeMake( (__size_in_points__).width * NS_CONTENT_SCALE_FACTOR(), (__size_in_points__).height * NS_CONTENT_SCALE_FACTOR())


#ifndef FLT_EPSILON
#define FLT_EPSILON     1.192092896e-07F
#endif // FLT_EPSILON

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
            TypeName(const TypeName&);\
            void operator=(const TypeName&)

/**
Helper macros which converts 4-byte little/big endian 
integral number to the machine native number representation
 
It should work same as apples CFSwapInt32LittleToHost(..)
*/

/// when define returns true it means that our architecture uses big endian
#define NS_HOST_IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) 
#define NS_SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#define NS_SWAP16(i)  ((i & 0x00ff) << 8 | (i &0xff00) >> 8)   
#define NS_SWAP_INT32_LITTLE_TO_HOST(i) ((NS_HOST_IS_BIG_ENDIAN == true)? NS_SWAP32(i) : (i) )
#define NS_SWAP_INT16_LITTLE_TO_HOST(i) ((NS_HOST_IS_BIG_ENDIAN == true)? NS_SWAP16(i) : (i) )
#define NS_SWAP_INT32_BIG_TO_HOST(i)    ((NS_HOST_IS_BIG_ENDIAN == true)? (i) : NS_SWAP32(i) )
#define NS_SWAP_INT16_BIG_TO_HOST(i)    ((NS_HOST_IS_BIG_ENDIAN == true)? (i):  NS_SWAP16(i) )

/**********************/
/** Profiling Macros **/
/**********************/
#if NS_ENABLE_PROFILERS

#define NS_PROFILER_DISPLAY_TIMERS() NSProfiler::sharedProfiler()->displayTimers()
#define NS_PROFILER_PURGE_ALL() NSProfiler::sharedProfiler()->releaseAllTimers()

#define NS_PROFILER_START(__name__) NSProfilingBeginTimingBlock(__name__)
#define NS_PROFILER_STOP(__name__) NSProfilingEndTimingBlock(__name__)
#define NS_PROFILER_RESET(__name__) NSProfilingResetTimingBlock(__name__)

#define NS_PROFILER_START_CATEGORY(__cat__, __name__) do{ if(__cat__) NSProfilingBeginTimingBlock(__name__); } while(0)
#define NS_PROFILER_STOP_CATEGORY(__cat__, __name__) do{ if(__cat__) NSProfilingEndTimingBlock(__name__); } while(0)
#define NS_PROFILER_RESET_CATEGORY(__cat__, __name__) do{ if(__cat__) NSProfilingResetTimingBlock(__name__); } while(0)

#define NS_PROFILER_START_INSTANCE(__id__, __name__) do{ NSProfilingBeginTimingBlock( [NSString stringWithFormat:@"%08X - %@", __id__, __name__] ); } while(0)
#define NS_PROFILER_STOP_INSTANCE(__id__, __name__) do{ NSProfilingEndTimingBlock(    [NSString stringWithFormat:@"%08X - %@", __id__, __name__] ); } while(0)
#define NS_PROFILER_RESET_INSTANCE(__id__, __name__) do{ NSProfilingResetTimingBlock( [NSString stringWithFormat:@"%08X - %@", __id__, __name__] ); } while(0)


#else

#define NS_PROFILER_DISPLAY_TIMERS() do {} while (0)
#define NS_PROFILER_PURGE_ALL() do {} while (0)

#define NS_PROFILER_START(__name__)  do {} while (0)
#define NS_PROFILER_STOP(__name__) do {} while (0)
#define NS_PROFILER_RESET(__name__) do {} while (0)

#define NS_PROFILER_START_CATEGORY(__cat__, __name__) do {} while(0)
#define NS_PROFILER_STOP_CATEGORY(__cat__, __name__) do {} while(0)
#define NS_PROFILER_RESET_CATEGORY(__cat__, __name__) do {} while(0)

#define NS_PROFILER_START_INSTANCE(__id__, __name__) do {} while(0)
#define NS_PROFILER_STOP_INSTANCE(__id__, __name__) do {} while(0)
#define NS_PROFILER_RESET_INSTANCE(__id__, __name__) do {} while(0)

#endif

#if !defined(NSECTA_DEBUG) || NSECTA_DEBUG == 0
#define CHECK_GL_ERROR_DEBUG()
#else
#define CHECK_GL_ERROR_DEBUG() \
    do { \
        GLenum __error = glGetError(); \
        if(__error) { \
            NSLog("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
        } \
    } while (false)
#endif

/** @def NS_INCREMENT_GL_DRAWS_BY_ONE
 Increments the GL Draws counts by one.
 The number of calls per frame are displayed on the screen when the NSDirector's stats are enabled.
 */
extern unsigned int g_uNumberOfDraws;
#define NS_INCREMENT_GL_DRAWS(__n__) g_uNumberOfDraws += __n__

/*******************/
/** Notifications **/
/*******************/
/** @def NSAnimationFrameDisplayedNotification
 Notification name when a NSSpriteFrame is displayed
 */
#define NSAnimationFrameDisplayedNotification "NSAnimationFrameDisplayedNotification"

#endif // __NSMACROS_H__