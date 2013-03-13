/****************************************************************************

 ****************************************************************************/
#ifndef __NS_PLATFORM_MACROS_H__
#define __NS_PLATFORM_MACROS_H__

/**
 * define some platform specific macros
 */

#ifdef _WINDOWS

#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_SECURE_NO_WARNINGS 

#else
#include <android/log.h>
#endif

#include "nsConfig.h"
#include "NSPlatformConfig.h"
#include "NSPlatformDefine.h"

/**
 * define a create function for a specific type, such as NSLayer
 * @__TYPE__ class type to add create(), such as NSLayer
 */
#define CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

/**
 * define a node function for a specific type, such as NSLayer
 * @__TYPE__ class type to add node(), such as NSLayer
 * @deprecated: This interface will be deprecated sooner or later.
 */
#define NODE_FUNC(__TYPE__) \
NS_DEPRECATED_ATTRIBUTE static __TYPE__* node() \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

/** @def NS_ENABLE_CACHE_TEXTURE_DATA
Enable it if you want to cache the texture data.
Basically,it's only enabled in android

It's new in EJECTA-x since v0.99.5
*/
#if (NS_TARGET_PLATFORM == NS_PLATFORM_ANDROID)
    #define NS_ENABLE_CACHE_TEXTURE_DATA       1
#else
    #define NS_ENABLE_CACHE_TEXTURE_DATA       0
#endif

#if (NS_TARGET_PLATFORM == NS_PLATFORM_ANDROID) || (NS_TARGET_PLATFORM == NS_PLATFORM_WIN32)
    /* Application will crash in glDrawElements function on some win32 computers and some android devices.
       Indices should be bound again while drawing to avoid this bug.
     */
    #define NS_REBIND_INDICES_BUFFER  1
#else
    #define NS_REBIND_INDICES_BUFFER  0
#endif

// generic macros

// namespace EJECTA {}
#ifdef __cplusplus
    #define NS_NS_BEGIN                     //namespace EJECTA {
    #define NS_NS_END                       //}
    #define USING_NS_NS                     //using namespace EJECTA
#else
    #define NS_NS_BEGIN 
    #define NS_NS_END 
    #define USING_NS_NS 
#endif 

/** NS_PROPERTY_READONLY is used to declare a protected variable.
 We can use getter to read the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 @warning : The getter is a public virtual function, you should rewrite it first.
 The variables and methods declared after NS_PROPERTY_READONLY are all public.
 If you need protected or private, please declare.
 */
#define NS_PROPERTY_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);

#define NS_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void);

/** NS_PROPERTY is used to declare a protected variable.
 We can use getter to read the variable, and use the setter to change the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 "set + funName" is the name of the setter.
 @warning : The getter and setter are public virtual functions, you should rewrite them first.
 The variables and methods declared after NS_PROPERTY are all public.
 If you need protected or private, please declare.
 */
#define NS_PROPERTY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);\
public: virtual void set##funName(varType var);

#define NS_PROPERTY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void);\
public: virtual void set##funName(const varType& var);

/** NS_SYNTHESIZE_READONLY is used to declare a protected variable.
 We can use getter to read the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 @warning : The getter is a public inline function.
 The variables and methods declared after NS_SYNTHESIZE_READONLY are all public.
 If you need protected or private, please declare.
 */
#define NS_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }

#define NS_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }

/** NS_SYNTHESIZE is used to declare a protected variable.
 We can use getter to read the variable, and use the setter to change the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 "set + funName" is the name of the setter.
 @warning : The getter and setter are public  inline functions.
 The variables and methods declared after NS_SYNTHESIZE are all public.
 If you need protected or private, please declare.
 */
#define NS_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define NS_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }\
public: virtual void set##funName(const varType& var){ varName = var; }

#define NS_SYNTHESIZE_RETAIN(varType, varName, funName)    \
private: varType varName; \
public: virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var)   \
{ \
    if (varName != var) \
    { \
        NS_SAFE_RETAIN(var); \
        NS_SAFE_RELEASE(varName); \
        varName = var; \
    } \
} 

#define NS_SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)
#define NS_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)
#define NS_SAFE_FREE(p)                do { if(p) { free(p); (p) = 0; } } while(0)
#define NS_SAFE_RELEASE(p)            do { if(p) { (p)->release(); } } while(0)
#define NS_SAFE_RELEASE_NULL(p)        do { if(p) { (p)->release(); (p) = 0; } } while(0)
#define NS_SAFE_RETAIN(p)            do { if(p) { (p)->retain(); } } while(0)
#define NS_BREAK_IF(cond)            if(cond) break

#define  LOG_TAG    "ejecta"

#ifdef _WINDOWS	
#define  NSLog(format, ...)      if(format){char tmpText[1024];sprintf(tmpText,format, __VA_ARGS__);OutputDebugStringA(tmpText);}OutputDebugStringA("\n")
#else
#define  NSLog(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#endif

#define __NSLOGWITHFUNCTION(s, ...) \
    NSLog("%s : %s",__FUNCTION__, NSString::createWithFormat(s, ##__VA_ARGS__)->getCString())

// EJECTA debug
#if !defined(EJECTA_DEBUG) || EJECTA_DEBUG == 0
#define NSLOG(...)       do {} while (0)
#define NSLOGINFO(...)   do {} while (0)
#define NSLOGERROR(...)  do {} while (0)
#define NSLOGWARN(...)   do {} while (0)

#elif EJECTA_DEBUG == 1
#define NSLOG(format, ...)      NSLog(format, ##__VA_ARGS__)
#define NSLOGERROR(format,...)  NSLog(format, ##__VA_ARGS__)
#define NSLOGINFO(format,...)   do {} while (0)
#define NSLOGWARN(...) __NSLOGWITHFUNCTION(__VA_ARGS__)

#elif EJECTA_DEBUG > 1
#define NSLOG(format, ...)      NSLog(format, ##__VA_ARGS__)
#define NSLOGERROR(format,...)  NSLog(format, ##__VA_ARGS__)
#define NSLOGINFO(format,...)   NSLog(format, ##__VA_ARGS__)
#define NSLOGWARN(...) __NSLOGWITHFUNCTION(__VA_ARGS__)
#endif // EJECTA_DEBUG

// Lua engine debug
#if !defined(EJECTA_DEBUG) || EJECTA_DEBUG == 0 || NS_LUA_ENGINE_DEBUG == 0
#define LUALOG(...)
#else
#define LUALOG(format, ...)     NSLog(format, ##__VA_ARGS__)
#endif // Lua engine debug

/*
 * only certain compilers support __attribute__((deprecated))
 */
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define NS_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
    #define NS_DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
    #define NS_DEPRECATED_ATTRIBUTE
#endif 

#endif // __NS_PLATFORM_MACROS_H__
