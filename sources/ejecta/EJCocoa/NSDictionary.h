/****************************************************************************

****************************************************************************/

#ifndef __NSDICTIONARY_H__
#define __NSDICTIONARY_H__

#include "support/uthash.h"
#include "NSObject.h"
#include "NSArray.h"
#include "NSString.h"

NS_NS_BEGIN

class NSDictionary;

/**
 * @addtogroup data_structures
 * @{
 */

class NS_DLL NSDictElement
{
    #define MAX_KEY_LEN  256
public:
    NSDictElement(const char* pszKey, NSObject* pObject)
    {
        init();
        m_pObject = pObject;

        const char* pStart = pszKey;
        
        int len = strlen(pszKey);
        if (len > MAX_KEY_LEN )
        {
            char* pEnd = (char*)&pszKey[len-1];
            pStart = pEnd - (MAX_KEY_LEN-1);
        }

        strcpy(m_szKey, pStart);
    }

    NSDictElement(intptr_t iKey, NSObject* pObject)
    {
        init();
        m_iKey = iKey;
        m_pObject = pObject;
    }

    inline const char* getStrKey() const
    {
        NSAssert(m_szKey[0] != '\0', "Should not call this function for integer dictionary");
        return m_szKey;
    }

    inline intptr_t getIntKey() const 
    {
        NSAssert(m_szKey[0] == '\0', "Should not call this function for string dictionary");
        return m_iKey;
    }

    inline NSObject* getObject() const
    {
        return m_pObject;
    }

private:
    inline void init()
    {
        m_iKey = 0;
        m_pObject = NULL;
        memset(m_szKey, 0, sizeof(m_szKey));
        memset(&hh, 0, sizeof(hh));
    }

private:
    char m_szKey[MAX_KEY_LEN+1];  /** hash key of string type*/
    intptr_t  m_iKey;   /** hash key of integer type */
    NSObject* m_pObject;/** hash value */
public:
    UT_hash_handle hh; /* makes this class hashable */
    friend class NSDictionary;
};


#define NSDICT_FOREACH(__dict__, __el__) \
    NSDictElement* pTmp##__dict__##__el__ = NULL; \
    HASH_ITER(hh, (__dict__)->m_pElements, __el__, pTmp##__dict__##__el__)


class NS_DLL NSDictionary : public NSObject
{
public:
    NSDictionary();
    ~NSDictionary();

    /// return the number of items
    unsigned int count();

    /// return all the keys
    NSArray* allKeys();

    /** @warning : We use '==' to compare two objects*/
    NSArray* allKeysForObject(NSObject* object);

    NSObject* objectForKey(const std::string& key);
    NSObject* objectForKey(int key);
    const NSString* valueForKey(const std::string& key);
    const NSString* valueForKey(int key);


    void setObject(NSObject* pObject, const std::string& key);
    void setObject(NSObject* pObject, int key);

    void removeObjectForKey(const std::string& key);
    void removeObjectForKey(int key);
    void removeObjectsForKeys(NSArray* pKeyArray);
    void removeObjectForElememt(NSDictElement* pElement);
    void removeAllObjects();

    virtual NSObject* copyWithZone(NSZone* pZone);

    /* static functions */
    //@deprecated: Please use create() instead. This interface will be deprecated sooner or later.
    NS_DEPRECATED_ATTRIBUTE static NSDictionary* dictionary();

    //@deprecated: Please use createWithDictionary(NSDictionary*) instead. This interface will be deprecated sooner or later.
    NS_DEPRECATED_ATTRIBUTE static NSDictionary* dictionaryWithDictionary(NSDictionary* srcDict);
    
    /**
    @brief   Generate a NSDictionary pointer by file
    @param   pFileName  The file name of *.plist file
    @return  The NSDictionary pointer generated from the file
    @deprecated: Please use createWithContentsOfFile(const char*) instead. This interface will be deprecated sooner or later.
    */
    NS_DEPRECATED_ATTRIBUTE static NSDictionary* dictionaryWithContentsOfFile(const char *pFileName);

    /*
    @brief The same meaning as dictionaryWithContentsOfFile(), but it doesn't call autorelease, so the
           invoker should call release().
    @deprecated: Please use createWithContentsOfFileThreadSafe(const char*) instead. This interface will be deprecated sooner or later.
    */
    NS_DEPRECATED_ATTRIBUTE static NSDictionary* dictionaryWithContentsOfFileThreadSafe(const char *pFileName);

    static NSDictionary* create();

    static NSDictionary* createWithDictionary(NSDictionary* srcDict);
    /**
    @brief   Generate a NSDictionary pointer by file
    @param   pFileName  The file name of *.plist file
    @return  The NSDictionary pointer generated from the file
    */
    static NSDictionary* createWithContentsOfFile(const char *pFileName);

    /*
    @brief The same meaning as dictionaryWithContentsOfFile(), but it doesn't call autorelease, so the
           invoker should call release().
    */
    static NSDictionary* createWithContentsOfFileThreadSafe(const char *pFileName);

private:
    void setObjectUnSafe(NSObject* pObject, const std::string& key);
    void setObjectUnSafe(NSObject* pObject, const int key);
    
public:
    NSDictElement* m_pElements;
private:
    
    enum NSDictType
    {
        kNSDictUnknown = 0,
        kNSDictStr,
        kNSDictInt
    };
    NSDictType m_eDictType;
    NSDictType m_eOldDictType;
};

// end of data_structure group
/// @}

NS_NS_END

#endif /* __NSDICTIONARY_H__ */
