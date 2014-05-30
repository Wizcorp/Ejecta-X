/****************************************************************************

****************************************************************************/

#include "NSSet.h"

using namespace std;

NS_NS_BEGIN

NSSet::NSSet(void)
{
    m_pSet = new set<NSObject *>;
}

NSSet::NSSet(const NSSet &rSetObject)
{
    m_pSet = new set<NSObject *>(*rSetObject.m_pSet);

    // call retain of members
    NSSetIterator iter;
    for (iter = m_pSet->begin(); iter != m_pSet->end(); ++iter)
    {
        if (! (*iter))
        {
            break;
        }

        (*iter)->retain();
    }
}

NSSet::~NSSet(void)
{
    // call release() of elements
    NSSetIterator iter;
    for (iter = m_pSet->begin(); iter != m_pSet->end(); ++iter)
    {
        if (! (*iter))
        {
            break;
        }

        (*iter)->release();
    }

    NS_SAFE_DELETE(m_pSet);
}

NSSet* NSSet::copy(void)
{
    NSSet *pSet = new NSSet(*this);

    return pSet;
}

NSSet* NSSet::mutableCopy(void)
{
    return copy();
}

int NSSet::count(void)
{
    return (int)m_pSet->size();
}

void NSSet::addObject(NSObject *pObject)
{
    NS_SAFE_RETAIN(pObject);
    m_pSet->insert(pObject);
}

void NSSet::removeObject(NSObject *pObject)
{
    m_pSet->erase(pObject);
    NS_SAFE_RELEASE(pObject);
}

bool NSSet::containsObject(NSObject *pObject)
{
    return m_pSet->find(pObject) != m_pSet->end();
}

NSSetIterator NSSet::begin(void)
{
    return m_pSet->begin();
}

NSSetIterator NSSet::end(void)
{
    return m_pSet->end();
}

NSObject* NSSet::anyObject()
{
    if (!m_pSet || m_pSet->empty())
    {
        return 0;
    }
    
    NSSetIterator it;

    for( it = m_pSet->begin(); it != m_pSet->end(); ++it)
    {
        if (*it)
        {
            return (*it);
        }
    }

    return 0;
}

NS_NS_END
