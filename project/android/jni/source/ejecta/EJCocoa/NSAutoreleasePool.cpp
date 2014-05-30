/****************************************************************************

****************************************************************************/
#include "NSAutoreleasePool.h"
#include "support/nsMacros.h"

NS_NS_BEGIN

static NSPoolManager* s_pPoolManager = NULL;

NSAutoreleasePool::NSAutoreleasePool(void)
{
    m_pManagedObjectArray = new NSArray();
    m_pManagedObjectArray->init();
}

NSAutoreleasePool::~NSAutoreleasePool(void)
{
    NS_SAFE_DELETE(m_pManagedObjectArray);
}

void NSAutoreleasePool::addObject(NSObject* pObject)
{
    m_pManagedObjectArray->addObject(pObject);

    NSAssert(pObject->m_uReference > 1, "reference count should be greater than 1");
    ++(pObject->m_uAutoReleaseCount);
    pObject->release(); // no ref count, in this case autorelease pool added.
}

void NSAutoreleasePool::removeObject(NSObject* pObject)
{
    for (unsigned int i = 0; i < pObject->m_uAutoReleaseCount; ++i)
    {
        m_pManagedObjectArray->removeObject(pObject, false);
    }
}

void NSAutoreleasePool::clear()
{
    if(m_pManagedObjectArray->count() > 0)
    {
        //NSAutoreleasePool* pReleasePool;
#ifdef _DEBUG
        int nIndex = m_pManagedObjectArray->count() - 1;
#endif

        NSObject* pObj = NULL;
        NSARRAY_FOREACH_REVERSE(m_pManagedObjectArray, pObj)
        {
            if(!pObj)
                break;

            --(pObj->m_uAutoReleaseCount);
            //(*it)->release();
            //delete (*it);
#ifdef _DEBUG
            nIndex--;
#endif
        }

        m_pManagedObjectArray->removeAllObjects();
    }
}


//--------------------------------------------------------------------
//
// NSPoolManager
//
//--------------------------------------------------------------------

NSPoolManager* NSPoolManager::sharedPoolManager()
{
    if (s_pPoolManager == NULL)
    {
        s_pPoolManager = new NSPoolManager();
    }
    return s_pPoolManager;
}

void NSPoolManager::purgePoolManager()
{
    NS_SAFE_DELETE(s_pPoolManager);
}

NSPoolManager::NSPoolManager()
{
    m_pReleasePoolStack = new NSArray();    
    m_pReleasePoolStack->init();
    m_pCurReleasePool = 0;
}

NSPoolManager::~NSPoolManager()
{
    
     finalize();
 
     // we only release the last autorelease pool here 
    m_pCurReleasePool = 0;
     m_pReleasePoolStack->removeObjectAtIndex(0);
 
     NS_SAFE_DELETE(m_pReleasePoolStack);
}

void NSPoolManager::finalize()
{
    if(m_pReleasePoolStack->count() > 0)
    {
        //NSAutoreleasePool* pReleasePool;
        NSObject* pObj = NULL;
        NSARRAY_FOREACH(m_pReleasePoolStack, pObj)
        {
            if(!pObj)
                break;
            NSAutoreleasePool* pPool = (NSAutoreleasePool*)pObj;
            pPool->clear();
        }
    }
}

void NSPoolManager::push()
{
    NSAutoreleasePool* pPool = new NSAutoreleasePool();       //ref = 1
    m_pCurReleasePool = pPool;

    m_pReleasePoolStack->addObject(pPool);                   //ref = 2

    pPool->release();                                       //ref = 1
}

void NSPoolManager::pop()
{
    if (! m_pCurReleasePool)
    {
        return;
    }

     int nCount = m_pReleasePoolStack->count();

    m_pCurReleasePool->clear();
 
      if(nCount > 1)
      {
        m_pReleasePoolStack->removeObjectAtIndex(nCount-1);

//         if(nCount > 1)
//         {
//             m_pCurReleasePool = m_pReleasePoolStack->objectAtIndex(nCount - 2);
//             return;
//         }
        m_pCurReleasePool = (NSAutoreleasePool*)m_pReleasePoolStack->objectAtIndex(nCount - 2);
    }

    /*m_pCurReleasePool = NULL;*/
}

void NSPoolManager::removeObject(NSObject* pObject)
{
    NSAssert(m_pCurReleasePool, "current auto release pool should not be null");

    m_pCurReleasePool->removeObject(pObject);
}

void NSPoolManager::addObject(NSObject* pObject)
{
    getCurReleasePool()->addObject(pObject);
}


NSAutoreleasePool* NSPoolManager::getCurReleasePool()
{
    if(!m_pCurReleasePool)
    {
        push();
    }

    NSAssert(m_pCurReleasePool, "current auto release pool should not be null");

    return m_pCurReleasePool;
}

NS_NS_END
