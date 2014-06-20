/****************************************************************************

****************************************************************************/

#ifndef __NSCONFIG_H__
#define __NSCONFIG_H__

#include "NSPlatformConfig.h"

/**
 @file
  (ns) configuration file
*/

/** @def NS_ENABLE_GL_STATE_CACHE
 If enabled,  will maintain an OpenGL state cache internally to avoid unnecessary switches.
 In order to use them, you have to use the following functions, instead of the the GL ones:
    - nsGLUseProgram() instead of glUseProgram()
    - nsGLDeleteProgram() instead of glDeleteProgram()
    - nsGLBlendFunc() instead of glBlendFunc()

 If this functionality is disabled, then nsGLUseProgram(), nsGLDeleteProgram(), nsGLBlendFunc() will call the GL ones, without using the cache.

 It is recommended to enable whenever possible to improve speed.
 If you are migrating your code from GL ES 1.1, then keep it disabled. Once all your code works as expected, turn it on.

 @since v2.0.0
 */
#ifndef NS_ENABLE_GL_STATE_CACHE
#define NS_ENABLE_GL_STATE_CACHE 1
#endif

/** @def NS_FIX_ARTIFACTS_BY_STRECHING_TEXEL
If enabled, the texture coordinates will be calculated by using this formula:
- texCoord.left = (rect.origin.x*2+1) / (texture.wide*2);
- texCoord.right = texCoord.left + (rect.size.width*2-2)/(texture.wide*2);

The same for bottom and top.

This formula prevents artifacts by using 99% of the texture.
The "correct" way to prevent artifacts is by using the spritesheet-artifact-fixer.py or a similar tool.

Affected nodes:
- NSSprite / NSSpriteBatchNode and subclasses: NSLabelBMFont, NSTMXTiledMap
- NSLabelAtlas
- NSQuadParticleSystem
- NSTileMap

To enabled set it to 1. Disabled by default.

@since v0.99.5
*/
#ifndef NS_FIX_ARTIFACTS_BY_STRECHING_TEXEL
#define NS_FIX_ARTIFACTS_BY_STRECHING_TEXEL 0
#endif

/** @def NS_DIRECTOR_FPS_INTERVAL
 Seconds between FPS updates.
 0.5 seconds, means that the FPS number will be updated every 0.5 seconds.
 Having a bigger number means a more reliable FPS
 
 Default value: 0.1f
 */
#ifndef NS_DIRECTOR_STATS_INTERVAL
#define NS_DIRECTOR_STATS_INTERVAL (0.1f)
#endif

/** @def NS_DIRECTOR_FPS_POSITION
 Position of the FPS

 Default: 0,0 (bottom-left corner)
 */
#ifndef NS_DIRECTOR_FPS_POSITION
#define NS_DIRECTOR_FPS_POSITION nsp(0,0)
#endif

/** @def NS_DIRECTOR_DISPATCH_FAST_EVENTS
 If enabled, and only when it is used with NSFastDirector, the main loop will wait 0.04 seconds to
 dispatch all the events, even if there are not events to dispatch.
 If your game uses lot's of events (eg: touches) it might be a good idea to enable this feature.
 Otherwise, it is safe to leave it disabled.
 
 To enable set it to 1. Disabled by default.
 
 @warning This feature is experimental
 */
#ifndef NS_DIRECTOR_DISPATCH_FAST_EVENTS
 #define NS_DIRECTOR_DISPATCH_FAST_EVENTS 0
#endif

/** @def NS_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
If enabled, -mac will run on the Display Link thread. If disabled -mac will run in its own thread.

If enabled, the images will be drawn at the "correct" time, but the events might not be very responsive.
If disabled, some frames might be skipped, but the events will be dispatched as they arrived.

To enable set it to a 1, to disable it set to 0. Enabled by default.

Only valid for -mac. Not supported on -ios.

*/
#ifndef NS_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD
#define NS_DIRECTOR_MAC_USE_DISPLAY_LINK_THREAD 1
#endif

/** @def NS_NODE_RENDER_SUBPIXEL
 If enabled, the NSNode objects (NSSprite, NSLabel,etc) will be able to render in subpixels.
 If disabled, integer pixels will be used.
 
 To enable set it to 1. Enabled by default.
 */
#ifndef NS_NODE_RENDER_SUBPIXEL
#define NS_NODE_RENDER_SUBPIXEL 1
#endif

/** @def NS_SPRITEBATCHNODE_RENDER_SUBPIXEL
 If enabled, the NSSprite objects rendered with NSSpriteBatchNode will be able to render in subpixels.
 If disabled, integer pixels will be used.
 
 To enable set it to 1. Enabled by default.
 */
#ifndef NS_SPRITEBATCHNODE_RENDER_SUBPIXEL
#define NS_SPRITEBATCHNODE_RENDER_SUBPIXEL    1
#endif

/** @def NS_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
 Use GL_TRIANGLE_STRIP instead of GL_TRIANGLES when rendering the texture atlas.
 It seems it is the recommend way, but it is much slower, so, enable it at your own risk
 
 To enable set it to a value different than 0. Disabled by default.

 */
#ifndef NS_TEXTURE_ATLAS_USE_TRIANGLE_STRIP
#define NS_TEXTURE_ATLAS_USE_TRIANGLE_STRIP 0
#endif

/** @def NS_TEXTURE_ATLAS_USE_VAO
 By default, NSTextureAtlas (used by many  classes) will use VAO (Vertex Array Objects).
 Apple recommends its usage but they might consume a lot of memory, specially if you use many of them.
 So for certain cases, where you might need hundreds of VAO objects, it might be a good idea to disable it.
 
 To disable it set it to 0. Enabled by default.
 
 */
#ifndef NS_TEXTURE_ATLAS_USE_VAO
    #if (NS_TARGET_PLATFORM == NS_PLATFORM_IOS) || (NS_TARGET_PLATFORM == NS_PLATFORM_MAC)
        #define NS_TEXTURE_ATLAS_USE_VAO 1
    #else
        /* Some Windows display adapter driver cannot support VAO. */
        /* Some android devices cannot support VAO very well, so we disable it by default for android platform. */
        /* Blackberry also doesn't support this feature. */
		#define NS_TEXTURE_ATLAS_USE_VAO 0
    #endif
#endif


/** @def NS_USE_LA88_LABELS
 If enabled, it will use LA88 (Luminance Alpha 16-bit textures) for NSLabelTTF objects.
 If it is disabled, it will use A8 (Alpha 8-bit textures).
 LA88 textures are 6% faster than A8 textures, but they will consume 2x memory.

 This feature is enabled by default.

 @since v0.99.5
 */
#ifndef NS_USE_LA88_LABELS
#define NS_USE_LA88_LABELS 1
#endif

/** @def NS_SPRITE_DEBUG_DRAW
 If enabled, all subclasses of NSSprite will draw a bounding box
 Useful for debugging purposes only. It is recommended to leave it disabled.
 
 To enable set it to a value different than 0. Disabled by default:
 0 -- disabled
 1 -- draw bounding box
 2 -- draw texture box
 0 -- disabled
 1 -- draw bounding box
 2 -- draw texture box
*/
#ifndef NS_SPRITE_DEBUG_DRAW
#define NS_SPRITE_DEBUG_DRAW 0
#endif

/** @def NS_SPRITEBATCHNODE_DEBUG_DRAW
If enabled, all subclasses of NSSprite that are rendered using an NSSpriteBatchNode draw a bounding box.
Useful for debugging purposes only. It is recommended to leave it disabled.

To enable set it to a value different than 0. Disabled by default.
*/
#ifndef NS_SPRITEBATCHNODE_DEBUG_DRAW
#define NS_SPRITEBATCHNODE_DEBUG_DRAW 0
#endif

/** @def NS_LABELBMFONT_DEBUG_DRAW
If enabled, all subclasses of NSLabelBMFont will draw a bounding box
Useful for debugging purposes only. It is recommended to leave it disabled.

To enable set it to a value different than 0. Disabled by default.
*/
#ifndef NS_LABELBMFONT_DEBUG_DRAW
#define NS_LABELBMFONT_DEBUG_DRAW 0
#endif

/** @def NS_LABELATLAS_DEBUG_DRAW
 If enabled, all subclasses of LabeltAtlas will draw a bounding box
 Useful for debugging purposes only. It is recommended to leave it disabled.
 
 To enable set it to a value different than 0. Disabled by default.
 */
#ifndef NS_LABELATLAS_DEBUG_DRAW
#define NS_LABELATLAS_DEBUG_DRAW 0
#endif

/** @def NS_ENABLE_PROFILERS
 If enabled, will activate various profilers within . This statistical data will be output to the console
 once per second showing average time (in milliseconds) required to execute the specific routine(s).
 Useful for debugging purposes only. It is recommended to leave it disabled.
 
 To enable set it to a value different than 0. Disabled by default.
 */
#ifndef NS_ENABLE_PROFILERS
#define NS_ENABLE_PROFILERS 0
#endif

/** Enable Lua engine debug log */
#ifndef NS_LUA_ENGINE_DEBUG
#define NS_LUA_ENGINE_DEBUG 0
#endif

#endif // __NSCONFIG_H__
