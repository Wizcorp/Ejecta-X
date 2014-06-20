/****************************************************************************

****************************************************************************/

#include <math.h>
#include "NSArray.h"

NS_NS_BEGIN


NSArray::NSArray()
: data(NULL)
{
    init();
}

NSArray::NSArray(unsigned int capacity)
: data(NULL)
{
    initWithCapacity(capacity);
}

NSArray* NSArray::array()
{
    return NSArray::create();
}

NSArray* NSArray::create()
{
    NSArray* pArray = new NSArray();

    if (pArray && pArray->init())
    {
        pArray->autorelease();
    }
    else
    {
        NS_SAFE_DELETE(pArray);
    }
    
    return pArray;
}

NSArray* NSArray::arrayWithObject(NSObject* pObject)
{
    return NSArray::createWithObject(pObject);
}

NSArray* NSArray::createWithObject(NSObject* pObject)
{
    NSArray* pArray = new NSArray();

    if (pArray && pArray->initWithObject(pObject))
    {
        pArray->autorelease();
    }
    else
    {
        NS_SAFE_DELETE(pArray);
    }

    return pArray;
}

NSArray* NSArray::arrayWithObjects(NSObject* pObject, ...)
{
    va_list args;
    va_start(args,pObject);
    
    NSArray* pArray = create();
    if (pArray && pObject)
    {
        pArray->addObject(pObject);
        NSObject *i = va_arg(args, NSObject*);
        while(i) 
        {
            pArray->addObject(i);
            i = va_arg(args, NSObject*);
        }
    }
    else
    {
        NS_SAFE_DELETE(pArray);
    }

    va_end(args);
    
    return pArray;
}

NSArray* NSArray::create(NSObject* pObject, ...)
{
    va_list args;
    va_start(args,pObject);

    NSArray* pArray = create();
    if (pArray && pObject)
    {
        pArray->addObject(pObject);
        NSObject *i = va_arg(args, NSObject*);
        while(i) 
        {
            pArray->addObject(i);
            i = va_arg(args, NSObject*);
        }
    }
    else
    {
        NS_SAFE_DELETE(pArray);
    }

    va_end(args);

    return pArray;
}

NSArray* NSArray::arrayWithCapacity(unsigned int capacity)
{
    return NSArray::createWithCapacity(capacity);
}

NSArray* NSArray::createWithCapacity(unsigned int capacity)
{
    NSArray* pArray = new NSArray();

    if (pArray && pArray->initWithCapacity(capacity))
    {
        pArray->autorelease();
    }
    else
    {
        NS_SAFE_DELETE(pArray);
    }

    return pArray;
}

NSArray* NSArray::arrayWithArray(NSArray* otherArray)
{
    return NSArray::createWithArray(otherArray);
}

NSArray* NSArray::createWithArray(NSArray* otherArray)
{
    NSArray* pRet = (NSArray*)otherArray->copy();
    pRet->autorelease();
    return pRet;
}

NSArray* NSArray::arrayWithContentsOfFile(const char* pFileName)
{
    return NSArray::createWithContentsOfFile(pFileName);
}

NSArray* NSArray::createWithContentsOfFile(const char* pFileName)
{
    NSArray* pRet = NSArray::createWithContentsOfFileThreadSafe(pFileName);
    if (pRet != NULL)
    {
        pRet->autorelease();
    }
    return pRet;
}

NSArray* nsFileUtils_arrayWithContentsOfFileThreadSafe(const char* pFileName)
{
        // NSSAXParser parser;

        // if (false == parser.init("UTF-8"))
        // {
        //     return NULL;
        // }
        // parser.setDelegator(this);

        // parser.parse(pFileName);
        // return m_pArray;
	return NULL;
}

NSArray* NSArray::arrayWithContentsOfFileThreadSafe(const char* pFileName)
{
    return NSArray::createWithContentsOfFileThreadSafe(pFileName);
}

NSArray* NSArray::createWithContentsOfFileThreadSafe(const char* pFileName)
{
    return nsFileUtils_arrayWithContentsOfFileThreadSafe(pFileName);
}

bool NSArray::init()
{
    return initWithCapacity(1);
}

bool NSArray::initWithObject(NSObject* pObject)
{
    nsArrayFree(data);
    bool bRet = initWithCapacity(1);
    if (bRet)
    {
        addObject(pObject);
    }
    return bRet;
}

/** Initializes an array with some objects */
bool NSArray::initWithObjects(NSObject* pObject, ...)
{
    nsArrayFree(data);
    bool bRet = false;
    do 
    {
        NS_BREAK_IF(pObject != NULL);

        va_list args;
        va_start(args, pObject);

        NSArray* pArray = new NSArray();
        if (pArray && pObject)
        {
            pArray->addObject(pObject);
            NSObject* i = va_arg(args, NSObject*);
            while(i) 
            {
                pArray->addObject(i);
                i = va_arg(args, NSObject*);
            }
            bRet = true;
        }
        else
        {
            NS_SAFE_DELETE(pArray);
        }
        va_end(args);

    } while (false);

    return bRet;
}

bool NSArray::initWithCapacity(unsigned int capacity)
{
    nsArrayFree(data);
    data = nsArrayNew(capacity);
    return true;
}

bool NSArray::initWithArray(NSArray* otherArray)
{
    nsArrayFree(data);
    bool bRet = false;
    do 
    {
        NS_BREAK_IF(! initWithCapacity(otherArray->data->num));

        addObjectsFromArray(otherArray);
        bRet = true;
    } while (0);
    
    return bRet;
}

unsigned int NSArray::count()
{
    return data->num;
}

unsigned int NSArray::capacity()
{
    return data->max;
}

unsigned int NSArray::indexOfObject(NSObject* object)
{
    return nsArrayGetIndexOfObject(data, object);
}

NSObject* NSArray::objectAtIndex(unsigned int index)
{
    NSAssert(index < data->num, "index out of range in objectAtIndex()");

    return data->arr[index];
}

NSObject* NSArray::lastObject()
{
    if( data->num > 0 )
        return data->arr[data->num-1];

    return NULL;
}

NSObject* NSArray::randomObject()
{
    if (data->num==0)
    {
        return NULL;
    }

    float r = NSRANDOM_0_1();
    
    if (r == 1) // to prevent from ansessing data-arr[data->num], out of range.
    {
        r = 0;
    }
    
    return data->arr[(int)(data->num * r)];
}

bool NSArray::containsObject(NSObject* object)
{
    return nsArrayContainsObject(data, object);
}

bool NSArray::isEqualToArray(NSArray* otherArray)
{
    for (unsigned int i = 0; i< this->count(); i++)
    {
        if (!this->objectAtIndex(i)->isEqual(otherArray->objectAtIndex(i)))
        {
            return false;
        }
    }
    return true;
}

void NSArray::addObject(NSObject* object)
{
    nsArrayAppendObjectWithResize(data, object);
}

void NSArray::addObjectsFromArray(NSArray* otherArray)
{
    nsArrayAppendArrayWithResize(data, otherArray->data);
}

void NSArray::insertObject(NSObject* object, unsigned int index)
{
    nsArrayInsertObjectAtIndex(data, object, index);
}

void NSArray::removeLastObject(bool bReleaseObj)
{
    NSAssert(data->num, "no objects added");
    nsArrayRemoveObjectAtIndex(data, data->num-1, bReleaseObj);
}

void NSArray::removeObject(NSObject* object, bool bReleaseObj/* = true*/)
{
    nsArrayRemoveObject(data, object, bReleaseObj);
}

void NSArray::removeObjectAtIndex(unsigned int index, bool bReleaseObj)
{
    nsArrayRemoveObjectAtIndex(data, index, bReleaseObj);
}

void NSArray::removeObjectsInArray(NSArray* otherArray)
{
    nsArrayRemoveArray(data, otherArray->data);
}

void NSArray::removeAllObjects()
{
    nsArrayRemoveAllObjects(data);
}

void NSArray::fastRemoveObjectAtIndex(unsigned int index)
{
    nsArrayFastRemoveObjectAtIndex(data, index);
}

void NSArray::fastRemoveObject(NSObject* object)
{
    nsArrayFastRemoveObject(data, object);
}

void NSArray::exchangeObject(NSObject* object1, NSObject* object2)
{
    unsigned int index1 = nsArrayGetIndexOfObject(data, object1);
    if(index1 == UINT_MAX)
    {
        return;
    }

    unsigned int index2 = nsArrayGetIndexOfObject(data, object2);
    if(index2 == UINT_MAX)
    {
        return;
    }

    nsArraySwapObjectsAtIndexes(data, index1, index2);
}

void NSArray::exchangeObjectAtIndex(unsigned int index1, unsigned int index2)
{
    nsArraySwapObjectsAtIndexes(data, index1, index2);
}

void NSArray::replaceObjectAtIndex(unsigned int index, NSObject* pObject, bool bReleaseObject/* = true*/)
{
    nsArrayInsertObjectAtIndex(data, pObject, index);
    nsArrayRemoveObjectAtIndex(data, index+1);
}

void NSArray::reverseObjects()
{
    if (data->num > 1)
    {
        // floorf(), since in the case of an even number, the number of swaps stays the same
        int count = (int) floorf(data->num/2.f); 
        unsigned int maxIndex = data->num - 1;

        for (int i = 0; i < count ; i++)
        {
            nsArraySwapObjectsAtIndexes(data, i, maxIndex);
            maxIndex--;
        }
    }
}

void NSArray::reduceMemoryFootprint()
{
    nsArrayShrink(data);
}

NSArray::~NSArray()
{
    nsArrayFree(data);
}

NSObject* NSArray::copyWithZone(NSZone* pZone)
{
    NSAssert(pZone == NULL, "NSArray should not be inherited.");
    NSArray* pArray = new NSArray();
    pArray->initWithCapacity(this->data->num > 0 ? this->data->num : 1);

    NSObject* pObj = NULL;
    NSObject* pTmpObj = NULL;
    NSARRAY_FOREACH(this, pObj)
    {
        pTmpObj = pObj->copy();
        pArray->addObject(pTmpObj);
        pTmpObj->release();
    }
    return pArray;
}

NS_NS_END
