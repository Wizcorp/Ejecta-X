/****************************************************************************

****************************************************************************/

#ifndef __NS_PLATFORM_CONFIG_H__
#define __NS_PLATFORM_CONFIG_H__

//////////////////////////////////////////////////////////////////////////
// pre configure
//////////////////////////////////////////////////////////////////////////

// define supported target platform macro which NS uses.
#define NS_PLATFORM_UNKNOWN            0
#define NS_PLATFORM_IOS                1
#define NS_PLATFORM_ANDROID            2
#define NS_PLATFORM_WIN32              3
#define NS_PLATFORM_MARMALADE          4
#define NS_PLATFORM_LINUX              5
#define NS_PLATFORM_BADA               6
#define NS_PLATFORM_BLACKBERRY         7
#define NS_PLATFORM_MAC                8

// Determine target platform by compile environment macro.
#define NS_TARGET_PLATFORM             NS_PLATFORM_UNKNOWN

// iphone
#if ! NS_TARGET_PLATFORM && (defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR))
    #undef  NS_TARGET_PLATFORM
    #define NS_TARGET_PLATFORM         NS_PLATFORM_IOS
    #define NS_SUPPORT_PVRTC
#endif

// android
#if ! NS_TARGET_PLATFORM && defined(ANDROID)
    #undef  NS_TARGET_PLATFORM
    #define NS_TARGET_PLATFORM         NS_PLATFORM_ANDROID
#endif

// win32
#if ! NS_TARGET_PLATFORM && (defined(WIN32) && defined(_WINDOWS))
    #undef  NS_TARGET_PLATFORM
    #define NS_TARGET_PLATFORM         NS_PLATFORM_WIN32
#endif

// linux
#if ! NS_TARGET_PLATFORM && defined(LINUX)
    #undef  NS_TARGET_PLATFORM
    #define NS_TARGET_PLATFORM         NS_PLATFORM_LINUX
#endif

// marmalade
#if ! NS_TARGET_PLATFORM && defined(MARMALADE)
#undef  NS_TARGET_PLATFORM
#define NS_TARGET_PLATFORM         NS_PLATFORM_MARMALADE
#endif

// bada
#if ! NS_TARGET_PLATFORM && defined(SHP)
#undef  NS_TARGET_PLATFORM
#define NS_TARGET_PLATFORM         NS_PLATFORM_BADA
#endif

// qnx
#if ! NS_TARGET_PLATFORM && defined(__QNX__)
    #undef  NS_TARGET_PLATFORM
    #define NS_TARGET_PLATFORM     NS_PLATFORM_BLACKBERRY
#endif

// mac
#if ! NS_TARGET_PLATFORM && defined(TARGET_OS_MAC)
    #undef  NS_TARGET_PLATFORM
    #define NS_TARGET_PLATFORM         NS_PLATFORM_MAC
    //#define NS_SUPPORT_PVRTC
#endif

//////////////////////////////////////////////////////////////////////////
// post configure
//////////////////////////////////////////////////////////////////////////

// check user set platform
#if ! NS_TARGET_PLATFORM
    #error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif 

#if (NS_TARGET_PLATFORM == NS_PLATFORM_WIN32)
#pragma warning (disable:4127)  
#endif  // NS_PLATFORM_WIN32

#endif  // __NS_PLATFORM_CONFIG_H__

