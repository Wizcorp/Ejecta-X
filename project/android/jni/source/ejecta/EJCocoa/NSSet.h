/****************************************************************************

****************************************************************************/

#ifndef __NS_SET_H__
#define __NS_SET_H__

#include <set>
#include "NSObject.h"

NS_NS_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

typedef std::set<NSObject *>::iterator NSSetIterator;

class NS_DLL NSSet : public NSObject
{
public:
    NSSet(void);
    NSSet(const NSSet &rSetObject);
    virtual ~NSSet(void);

    /**
    *@brief Return a copy of the NSSet, it will copy all the elements.
    */
    NSSet* copy();
    /**
    *@brief It is the same as copy().
    */
    NSSet* mutableCopy();
    /**
    *@brief Return the number of elements the NSSet contains.
    */
    int count();
    /**
    *@brief Add a element into NSSet, it will retain the element.
    */
    void addObject(NSObject *pObject);
    /**
    *@brief Remove the given element, nothing todo if no element equals pObject.
    */
    void removeObject(NSObject *pObject);
    /**
    *@brief Check if NSSet contains a element equals pObject.
    */
    bool containsObject(NSObject *pObject);
    /**
    *@brief Return the iterator that points to the first element.
    */
    NSSetIterator begin();
    /**
    *@brief Return the iterator that points to the position after the last element.
    */
    NSSetIterator end();
    /**
    *@brief Return the first element if it contains elements, or null if it doesn't contain any element.
    */
    NSObject* anyObject();

private:
    std::set<NSObject *> *m_pSet;
};

// end of data_structure group
/// @}

NS_NS_END

#endif // __NS_SET_H__

