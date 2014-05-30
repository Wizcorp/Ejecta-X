/****************************************************************************

****************************************************************************/
#ifndef __AUTORELEASEPOOL_H__
#define __AUTORELEASEPOOL_H__

#include "NSObject.h"
#include "NSArray.h"

NS_NS_BEGIN

/**
 * @addtogroup base_nodes
 * @{
 */

class NS_DLL NSAutoreleasePool : public NSObject
{
    NSArray*    m_pManagedObjectArray;    
public:
    NSAutoreleasePool(void);
    ~NSAutoreleasePool(void);

    void addObject(NSObject *pObject);
    void removeObject(NSObject *pObject);

    void clear();
};

class NS_DLL NSPoolManager
{
    NSArray*    m_pReleasePoolStack;    
    NSAutoreleasePool*                    m_pCurReleasePool;

    NSAutoreleasePool* getCurReleasePool();
public:
    NSPoolManager();
    ~NSPoolManager();
    void finalize();
    void push();
    void pop();

    void removeObject(NSObject* pObject);
    void addObject(NSObject* pObject);

    static NSPoolManager* sharedPoolManager();
    static void purgePoolManager();

    friend class NSAutoreleasePool;
};

// end of base_nodes group
/// @}

NS_NS_END

#endif //__AUTORELEASEPOOL_H__
