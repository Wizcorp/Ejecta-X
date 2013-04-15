/****************************************************************************

****************************************************************************/

#ifndef __NSOBJECT_H__
#define __NSOBJECT_H__

#include "support/NSPlatformMacros.h"
#include "NSObjectFactory.h"

NS_NS_BEGIN

/**
 * @addtogroup base_nodes
 * @{
 */

class NSZone;
class NSObject;
class NSNode;
class NSEvent;

class NS_DLL NSCopying
{
public:
    virtual NSObject* copyWithZone(NSZone* pZone);
};

class NS_DLL NSObject : public NSCopying
{
public:
    // object id, NSScriptSupport need public m_uID
    unsigned int        m_uID;
    // Lua reference id
    int                 m_nLuaID;
protected:
    // count of references
    unsigned int        m_uReference;
    // count of autorelease
    unsigned int        m_uAutoReleaseCount;
public:
    NSObject(void);
    virtual ~NSObject(void);
    
    void release(void);
    void retain(void);
    NSObject* autorelease(void);
    NSObject* copy(void);
    bool isSingleReference(void);
    unsigned int retainCount(void);
    virtual bool isEqual(const NSObject* pObject);

    virtual void update(float dt) {NS_UNUSED_PARAM(dt);};
    
    friend class NSAutoreleasePool;
};

template<class T>
class NSObjectT : virtual public NSObject, virtual public T{};

typedef void (NSObject::*SEL_SCHEDULE)(float);
typedef void (NSObject::*SEL_CallFunc)();
typedef void (NSObject::*SEL_CallFuncN)(NSNode*);
typedef void (NSObject::*SEL_CallFuncND)(NSNode*, void*);
typedef void (NSObject::*SEL_CallFuncO)(NSObject*);
typedef void (NSObject::*SEL_MenuHandler)(NSObject*);
typedef void (NSObject::*SEL_EventHandler)(NSEvent*);
typedef int (NSObject::*SEL_Compare)(NSObject*);

#define schedule_selector(_SELECTOR) (SEL_SCHEDULE)(&_SELECTOR)
#define callfunc_selector(_SELECTOR) (SEL_CallFunc)(&_SELECTOR)
#define callfuncN_selector(_SELECTOR) (SEL_CallFuncN)(&_SELECTOR)
#define callfuncND_selector(_SELECTOR) (SEL_CallFuncND)(&_SELECTOR)
#define callfuncO_selector(_SELECTOR) (SEL_CallFuncO)(&_SELECTOR)
#define menu_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)
#define event_selector(_SELECTOR) (SEL_EventHandler)(&_SELECTOR)
#define compare_selector(_SELECTOR) (SEL_Compare)(&_SELECTOR)

// end of base_nodes group
/// @}

NS_NS_END

#endif // __NSOBJECT_H__
