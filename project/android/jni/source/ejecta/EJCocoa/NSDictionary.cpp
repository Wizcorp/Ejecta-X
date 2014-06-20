#include "NSDictionary.h"
#include "NSString.h"
#include "NSInteger.h"

using namespace std;

NS_NS_BEGIN

NSDictionary::NSDictionary()
: m_pElements(NULL)
, m_eDictType(kNSDictUnknown)
, m_eOldDictType(kNSDictUnknown)
{

}

NSDictionary::~NSDictionary()
{
    removeAllObjects();
}

unsigned int NSDictionary::count()
{
    return HASH_COUNT(m_pElements);
}

NSArray* NSDictionary::allKeys()
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;

    NSArray* pArray = NSArray::createWithCapacity(iKeyCount);

    NSDictElement *pElement, *tmp;
    if (m_eDictType == kNSDictStr)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            NSString* pOneKey = new NSString(pElement->m_szKey);
            pOneKey->autorelease();
            pArray->addObject(pOneKey);
        }
    }
    else if (m_eDictType == kNSDictInt)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            NSInteger* pOneKey = new NSInteger(pElement->m_iKey);
            pOneKey->autorelease();
            pArray->addObject(pOneKey);
        }
    }
    
    return pArray;
}

NSArray* NSDictionary::allKeysForObject(NSObject* object)
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;
    NSArray* pArray = NSArray::create();

    NSDictElement *pElement, *tmp;

    if (m_eDictType == kNSDictStr)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            if (object == pElement->m_pObject)
            {
                NSString* pOneKey = new NSString(pElement->m_szKey);
                pArray->addObject(pOneKey);
                pOneKey->release();
            }
        }
    }
    else if (m_eDictType == kNSDictInt)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            if (object == pElement->m_pObject)
            {
                NSInteger* pOneKey = new NSInteger(pElement->m_iKey);
                pArray->addObject(pOneKey);
                pOneKey->release();
            }
        }
    }
    return pArray;
}

NSObject* NSDictionary::objectForKey(const std::string& key)
{
    // if dictionary wasn't initialized, return NULL directly.
    if (m_eDictType == kNSDictUnknown) return NULL;
    // NSDictionary only supports one kind of key, string or integer.
    // This method uses string as key, therefore we should make sure that the key type of this NSDictionary is string.
    NSAssert(m_eDictType == kNSDictStr, "this dictionary does not use string as key.");

    NSObject* pRetObject = NULL;
    NSDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement != NULL)
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

NSObject* NSDictionary::objectForKey(int key)
{
    // if dictionary wasn't initialized, return NULL directly.
    if (m_eDictType == kNSDictUnknown) return NULL;
    // NSDictionary only supports one kind of key, string or integer.
    // This method uses integer as key, therefore we should make sure that the key type of this NSDictionary is integer.
    NSAssert(m_eDictType == kNSDictInt, "this dictionary does not use integer as key.");

    NSObject* pRetObject = NULL;
    NSDictElement *pElement = NULL;
    HASH_FIND_INT(m_pElements, &key, pElement);
    if (pElement != NULL)
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

const NSString* NSDictionary::valueForKey(const std::string& key)
{
    NSString* pStr = (NSString*)objectForKey(key);
    if (pStr == NULL)
    {
        pStr = NSString::create("");
    }
    return pStr;
}

const NSString* NSDictionary::valueForKey(int key)
{
    NSString* pStr = (NSString*)objectForKey(key);
    if (pStr == NULL)
    {
        pStr = NSString::create("");
    }
    return pStr;
}

void NSDictionary::setObject(NSObject* pObject, const std::string& key)
{
    NSAssert(key.length() > 0 && pObject != NULL, "Invalid Argument!");
    if (m_eOldDictType == kNSDictUnknown)
    {
        m_eOldDictType = kNSDictStr;
    }
    m_eDictType = kNSDictStr;
    NSAssert(m_eDictType == m_eOldDictType, "this dictionary does not use string as key.");

    NSDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
        NSObject* pTmpObj = pElement->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pElement);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }
}

void NSDictionary::setObject(NSObject* pObject, int key)
{
    NSAssert(pObject != NULL, "Invalid Argument!");
    if (m_eOldDictType == kNSDictUnknown)
    {
        m_eOldDictType = kNSDictInt;
    }
    m_eDictType = kNSDictInt;
    NSAssert(m_eDictType == m_eOldDictType, "this dictionary does not use integer as key.");

    NSDictElement *pElement = NULL;
    HASH_FIND_INT(m_pElements, &key, pElement);
    if (pElement == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
        NSObject* pTmpObj = pElement->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pElement);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }

}

void NSDictionary::removeObjectForKey(const std::string& key)
{
    if (m_eOldDictType == kNSDictUnknown) 
    {
        return;
    }
    NSAssert(m_eDictType == kNSDictStr, "this dictionary does not use string as its key");
    NSAssert(key.length() > 0, "Invalid Argument!");
    NSDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    removeObjectForElememt(pElement);
}

void NSDictionary::removeObjectForKey(int key)
{
    if (m_eOldDictType == kNSDictUnknown) 
    {
        return;
    }
    NSAssert(m_eDictType == kNSDictInt, "this dictionary does not use integer as its key");
    NSDictElement *pElement = NULL;
    HASH_FIND_INT(m_pElements, &key, pElement);
    removeObjectForElememt(pElement);
}

void NSDictionary::setObjectUnSafe(NSObject* pObject, const std::string& key)
{
    pObject->retain();
    NSDictElement* pElement = new NSDictElement(key.c_str(), pObject);
    HASH_ADD_STR(m_pElements, m_szKey, pElement);
}

void NSDictionary::setObjectUnSafe(NSObject* pObject, const int key)
{
    pObject->retain();
    NSDictElement* pElement = new NSDictElement(key, pObject);
    HASH_ADD_INT(m_pElements, m_iKey, pElement);
}

void NSDictionary::removeObjectsForKeys(NSArray* pKeyArray)
{
    NSObject* pObj = NULL;
    NSARRAY_FOREACH(pKeyArray, pObj)
    {
        NSString* pStr = (NSString*)pObj;
        removeObjectForKey(pStr->getCString());
    }
}

void NSDictionary::removeObjectForElememt(NSDictElement* pElement)
{
    if (pElement != NULL)
    {
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        NS_SAFE_DELETE(pElement);
    }
}

void NSDictionary::removeAllObjects()
{
    NSDictElement *pElement, *tmp;
    HASH_ITER(hh, m_pElements, pElement, tmp) 
    {
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        NS_SAFE_DELETE(pElement);
    }
}

NSObject* NSDictionary::copyWithZone(NSZone* pZone)
{
    NSAssert(pZone == NULL, "NSDictionary should not be inherited.");
    NSDictionary* pNewDict = new NSDictionary();

    NSDictElement* pElement = NULL;
    NSObject* pTmpObj = NULL;

    if (m_eDictType == kNSDictInt)
    {
        NSDICT_FOREACH(this, pElement)
        {
            pTmpObj = pElement->getObject()->copy();
            pNewDict->setObject(pTmpObj, pElement->getIntKey());
            pTmpObj->release();
        }
    }
    else if (m_eDictType == kNSDictStr)
    {
        NSDICT_FOREACH(this, pElement)
        {
            pTmpObj = pElement->getObject()->copy();
            pNewDict->setObject(pTmpObj, pElement->getStrKey());
            pTmpObj->release();
        }
    }

    return pNewDict;
}

NSDictionary* NSDictionary::dictionary()
{
    return NSDictionary::create();
}

NSDictionary* NSDictionary::create()
{
    NSDictionary* pRet = new NSDictionary();
    if (pRet != NULL)
    {
        pRet->autorelease();
    }
    return pRet;
}

NSDictionary* NSDictionary::dictionaryWithDictionary(NSDictionary* srcDict)
{
    return NSDictionary::createWithDictionary(srcDict);
}

NSDictionary* NSDictionary::createWithDictionary(NSDictionary* srcDict)
{
    NSDictionary* pNewDict = (NSDictionary*)srcDict->copy();
    pNewDict->autorelease();
    return pNewDict;
}

NSDictionary* nsFileUtils_dictionaryWithContentsOfFileThreadSafe(const char *pFileName)
{
    //
	return NULL;
}

NSDictionary* NSDictionary::dictionaryWithContentsOfFileThreadSafe(const char *pFileName)
{
    return NSDictionary::createWithContentsOfFileThreadSafe(pFileName);
}

NSDictionary* NSDictionary::createWithContentsOfFileThreadSafe(const char *pFileName)
{
    return nsFileUtils_dictionaryWithContentsOfFileThreadSafe(pFileName);
}

NSDictionary* NSDictionary::dictionaryWithContentsOfFile(const char *pFileName)
{
    return NSDictionary::createWithContentsOfFile(pFileName);
}

NSDictionary* NSDictionary::createWithContentsOfFile(const char *pFileName)
{
    NSDictionary* pRet = createWithContentsOfFileThreadSafe(pFileName);
    pRet->autorelease();
    return pRet;
}

NS_NS_END
