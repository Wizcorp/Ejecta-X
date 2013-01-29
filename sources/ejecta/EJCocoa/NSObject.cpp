/****************************************************************************

****************************************************************************/


#include "NSObject.h"
#include "NSAutoreleasePool.h"
#include "support/nsMacros.h"
//#include "script_support/NSScriptSupport.h"

NS_NS_BEGIN

NSObject* NSCopying::copyWithZone(NSZone *pZone)
{
    NS_UNUSED_PARAM(pZone);
    NSAssert(0, "not implement");
    return 0;
}

NSObject::NSObject(void)
:m_uAutoReleaseCount(0)
,m_uReference(1) // when the object is created, the reference count of it is 1
,m_nLuaID(0)
{
    static unsigned int uObjectCount = 0;

    m_uID = ++uObjectCount;
}

NSObject::~NSObject(void)
{
    // if the object is managed, we should remove it
    // from pool manager
    if (m_uAutoReleaseCount > 0)
    {
        NSPoolManager::sharedPoolManager()->removeObject(this);
    }

    // if the object is referenced by Lua engine, remove it
    // if (m_nLuaID)
    // {
    //     NSScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptObjectByNSObject(this);
    // }
    // else
    // {
    //     NSScriptEngineProtocol* pEngine = NSScriptEngineManager::sharedManager()->getScriptEngine();
    //     if (pEngine != NULL && pEngine->getScriptType() == kScriptTypeJavascript)
    //     {
    //         pEngine->removeScriptObjectByNSObject(this);
    //     }
    // }
}

NSObject* NSObject::copy()
{
    return copyWithZone(0);
}

void NSObject::release(void)
{
    NSAssert(m_uReference > 0, "reference count should greater than 0");
    --m_uReference;

    if (m_uReference == 0)
    {
        delete this;
    }
}

void NSObject::retain(void)
{
    NSAssert(m_uReference > 0, "reference count should greater than 0");

    ++m_uReference;
}

NSObject* NSObject::autorelease(void)
{
    NSPoolManager::sharedPoolManager()->addObject(this);
    return this;
}

bool NSObject::isSingleReference(void)
{
    return m_uReference == 1;
}

unsigned int NSObject::retainCount(void)
{
    return m_uReference;
}

bool NSObject::isEqual(const NSObject *pObject)
{
    return this == pObject;
}

NS_NS_END
