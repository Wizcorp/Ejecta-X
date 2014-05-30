/****************************************************************************

****************************************************************************/
#ifndef __NSARRAY_H__
#define __NSARRAY_H__

#include <stdarg.h>
#include "support/nsCArray.h"

/**
 * @addtogroup data_structures
 * @{
 */

/** @def NSARRAY_FOREACH
A convenience macro to iterate over a NSArray using. It is faster than the "fast enumeration" interface.
@since v0.99.4
*/

/*
In iphone, This macro have been update to like this:

#define NSARRAY_FOREACH(__array__, __object__)                                                \
if (__array__ && __array__->data->num > 0)                                                    \
for(id *__arr__ = __array__->data->arr, *end = __array__->data->arr + __array__->data->num-1;    \
__arr__ <= end && ((__object__ = *__arr__) != nil || true);                                        \
__arr__++)

I found that it's not work in C++. So it keep what it's look like in version 1.0.0-rc3. ---By Bin
*/
#define NSARRAY_FOREACH(__array__, __object__)                                                                \
    if ((__array__) && (__array__)->data->num > 0)                                                            \
    for(NSObject** arr = (__array__)->data->arr, **end = (__array__)->data->arr + (__array__)->data->num-1;    \
    arr <= end && (((__object__) = *arr) != NULL/* || true*/);                                                \
    arr++)

#define NSARRAY_FOREACH_REVERSE(__array__, __object__)                                                      \
    if ((__array__) && (__array__)->data->num > 0)                                                          \
    for(NSObject** arr = (__array__)->data->arr + (__array__)->data->num-1, **end = (__array__)->data->arr; \
    arr >= end && (((__object__) = *arr) != NULL/* || true*/);                                              \
    arr--)

#if defined(NSECTA_DEBUG) && (NSECTA_DEBUG > 0)
#define NSARRAY_VERIFY_TYPE(__array__, __type__)                                                  \
    do {                                                                                          \
        if ((__array__) && (__array__)->data->num > 0)                                              \
            for(NSObject** arr = (__array__)->data->arr,                                          \
                **end = (__array__)->data->arr + (__array__)->data->num-1; arr <= end; arr++)     \
                NSAssert(dynamic_cast<__type__>(*arr), "element type is wrong!");                 \
    } while(false)
#else
#define NSARRAY_VERIFY_TYPE(__array__, __type__) void(0)
#endif

#define arrayMakeObjectsPerformSelector(pArray, func, elementType)    \
do {                                                                  \
    if(pArray && pArray->count() > 0)                                 \
    {                                                                 \
        NSObject* child;                                              \
        NSARRAY_FOREACH(pArray, child)                                \
        {                                                             \
            elementType pNode = (elementType) child;                  \
            if(pNode)                                                 \
            {                                                         \
                pNode->func();                                        \
            }                                                         \
        }                                                             \
    }                                                                 \
}                                                                     \
while(false)

#define arrayMakeObjectsPerformSelectorWithObject(pArray, func, pObject, elementType)   \
do {                                                                  \
    if(pArray && pArray->count() > 0)                                 \
    {                                                                 \
        NSObject* child = NULL;                                       \
        NSARRAY_FOREACH(pArray, child)                                \
        {                                                             \
            elementType pNode = (elementType) child;                  \
            if(pNode)                                                 \
            {                                                         \
                pNode->func(pObject);                                 \
            }                                                         \
        }                                                             \
    }                                                                 \
}                                                                     \
while(false)


NS_NS_BEGIN

class NS_DLL NSArray : public NSObject
{
public:
    ~NSArray();

    /* static functions */
    /** Create an array 
    @deprecated: Please use create() instead. This interface will be deprecated sooner or later.
    */
    NS_DEPRECATED_ATTRIBUTE static NSArray* array();
    /** Create an array with one object 
    @deprecated: Please use createWithObject(NSObject*) instead. This interface will be deprecated sooner or later.
    */
    NS_DEPRECATED_ATTRIBUTE static NSArray* arrayWithObject(NSObject* pObject);
    /** Create an array with some objects 
    @deprecated: Please use create(NSObject*, ...) instead. This interface will be deprecated sooner or later.
    */
    NS_DEPRECATED_ATTRIBUTE static NSArray* arrayWithObjects(NSObject* pObject, ...);
    /** Create an array with capacity 
    @deprecated: Please use createWithCapacity(unsigned int) instead. This interface will be deprecated sooner or later.
    */
    NS_DEPRECATED_ATTRIBUTE static NSArray* arrayWithCapacity(unsigned int capacity);
    /** Create an array with an existing array
    @deprecated: Please use createWithArray(NSArray*) instead. This interface will be deprecated sooner or later.
    */
    NS_DEPRECATED_ATTRIBUTE static NSArray* arrayWithArray(NSArray* otherArray);
    /**
     @brief   Generate a NSArray pointer by file
     @param   pFileName  The file name of *.plist file
     @return  The NSArray pointer generated from the file
     @deprecated: Please use createWithContentsOfFile(const char*) instead. This interface will be deprecated sooner or later.
     */
     NS_DEPRECATED_ATTRIBUTE static NSArray* arrayWithContentsOfFile(const char* pFileName);
    
    /*
     @brief The same meaning as arrayWithContentsOfFile(), but it doesn't call autorelease, so the
     invoker should call release().
     @deprecated: Please use createWithContentsOfFileThreadSafe(const char*) instead. This interface will be deprecated sooner or later.
     */
    NS_DEPRECATED_ATTRIBUTE static NSArray* arrayWithContentsOfFileThreadSafe(const char* pFileName);

    /** Create an array */
    static NSArray* create();
    /** Create an array with some objects */
    static NSArray* create(NSObject* pObject, ...);
    /** Create an array with one object */
    static NSArray* createWithObject(NSObject* pObject);
    /** Create an array with capacity */
    static NSArray* createWithCapacity(unsigned int capacity);
    /** Create an array with an existing array */
    static NSArray* createWithArray(NSArray* otherArray);
    /**
     @brief   Generate a NSArray pointer by file
     @param   pFileName  The file name of *.plist file
     @return  The NSArray pointer generated from the file
     */
    static NSArray* createWithContentsOfFile(const char* pFileName);
    
    /*
     @brief The same meaning as arrayWithContentsOfFile(), but it doesn't call autorelease, so the
     invoker should call release().
     */
    static NSArray* createWithContentsOfFileThreadSafe(const char* pFileName);

    /** Initializes an array */
    bool init();
    /** Initializes an array with one object */
    bool initWithObject(NSObject* pObject);
    /** Initializes an array with some objects */
    bool initWithObjects(NSObject* pObject, ...);
    /** Initializes an array with capacity */
    bool initWithCapacity(unsigned int capacity);
    /** Initializes an array with an existing array */
    bool initWithArray(NSArray* otherArray);

    // Querying an Array

    /** Returns element count of the array */
    unsigned int count();
    /** Returns capacity of the array */
    unsigned int capacity();
    /** Returns index of a certain object, return UINT_MAX if doesn't contain the object */
    unsigned int indexOfObject(NSObject* object);
    /** Returns an element with a certain index */
    NSObject* objectAtIndex(unsigned int index);
    /** Returns last element */
    NSObject* lastObject();
    /** Returns a random element */
    NSObject* randomObject();
    /** Returns a Boolean value that indicates whether object is present in array. */
    bool containsObject(NSObject* object);
    /** @since 1.1 */
    bool isEqualToArray(NSArray* pOtherArray);
    // Adding Objects

    /** Add a certain object */
    void addObject(NSObject* object);
    /** Add all elements of an existing array */
    void addObjectsFromArray(NSArray* otherArray);
    /** Insert a certain object at a certain index */
    void insertObject(NSObject* object, unsigned int index);

    // Removing Objects

    /** Remove last object */
    void removeLastObject(bool bReleaseObj = true);
    /** Remove a certain object */
    void removeObject(NSObject* object, bool bReleaseObj = true);
    /** Remove an element with a certain index */
    void removeObjectAtIndex(unsigned int index, bool bReleaseObj = true);
    /** Remove all elements */
    void removeObjectsInArray(NSArray* otherArray);
    /** Remove all objects */
    void removeAllObjects();
    /** Fast way to remove a certain object */
    void fastRemoveObject(NSObject* object);
    /** Fast way to remove an element with a certain index */
    void fastRemoveObjectAtIndex(unsigned int index);

    // Rearranging Content

    /** Swap two elements */
    void exchangeObject(NSObject* object1, NSObject* object2);
    /** Swap two elements with certain indexes */
    void exchangeObjectAtIndex(unsigned int index1, unsigned int index2);

    /** Replace object at index with another object. */
    void replaceObjectAtIndex(unsigned int uIndex, NSObject* pObject, bool bReleaseObject = true);

    /** Revers the array */
    void reverseObjects();
    /* Shrinks the array so the memory footprint corresponds with the number of items */
    void reduceMemoryFootprint();
  
    /* override functions */
    virtual NSObject* copyWithZone(NSZone* pZone);

public:
    nsArray* data;
    NSArray();
    NSArray(unsigned int capacity);
};

// end of data_structure group
/// @}

NS_NS_END

#endif // __NSARRAY_H__
