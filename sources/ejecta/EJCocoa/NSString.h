/****************************************************************************

****************************************************************************/
#ifndef __NSSTRING_H__
#define __NSSTRING_H__

#if (NS_TARGET_PLATFORM == NS_PLATFORM_BLACKBERRY)
#include <string.h>
#endif

#include <stdarg.h>
#include <string>
#include <functional>
#include "NSObject.h"

NS_NS_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

class NS_DLL NSString : public NSObject
{
public:
    NSString();
    NSString(const char* str);
    NSString(const std::string& str);
    NSString(const NSString& str);

    virtual ~NSString();
    
    /* override assignment operator */
    NSString& operator= (const NSString& other);

    /** init a string with format, it's similar with the c function 'sprintf' */ 
    bool initWithFormat(const char* format, ...);

    /** convert to int value */
    int intValue() const;

    /** convert to unsigned int value */
    unsigned int uintValue() const;

    /** convert to float value */
    float floatValue() const;

    /** convert to double value */
    double doubleValue() const;

    /** convert to bool value */
    bool boolValue() const;

    /** get the C string */
    const char* getCString() const;

    /** get the Extension of path */
    const char* pathExtension() const;

    /** get the length of string */
    unsigned int length() const;

    /** compare to a c string */
    int compare(const char *) const;

    /* override functions */
    virtual NSObject* copyWithZone(NSZone* pZone);
    virtual bool isEqual(const NSObject* pObject);

    /* static functions */
    /** create a string with c string
     *  @return A NSString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     @deprecated: This interface will be deprecated sooner or later.
     */
    NS_DEPRECATED_ATTRIBUTE static NSString* stringWithCString(const char* pStr);

    /** create a string with std::string
     *  @return A NSString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    NS_DEPRECATED_ATTRIBUTE static NSString* stringWithString(const std::string& str);

    /** create a string with format, it's similar with the c function 'sprintf', the default buffer size is (1024*100) bytes,
     *  if you want to change it, you should modify the kMaxStringLen macro in NSString.cpp file.
     *  @return A NSString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     @deprecated: This interface will be deprecated sooner or later.
     */ 
    NS_DEPRECATED_ATTRIBUTE static NSString* stringWithFormat(const char* format, ...);

    /** create a string with binary data 
     *  @return A NSString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     @deprecated: This interface will be deprecated sooner or later.
     */
    NS_DEPRECATED_ATTRIBUTE static NSString* stringWithData(const unsigned char* pData, unsigned long nLen);

    /** create a string with a file, 
     *  @return A NSString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     @deprecated: This interface will be deprecated sooner or later.
     */
    NS_DEPRECATED_ATTRIBUTE static NSString* stringWithContentsOfFile(const char* pszFileName);

    /** create a string with std string, you can also pass a c string pointer because the default constructor of std::string can ansess a c string pointer. 
     *  @return A NSString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static NSString* create(const std::string& str);

    /** create a string with format, it's similar with the c function 'sprintf', the default buffer size is (1024*100) bytes,
     *  if you want to change it, you should modify the kMaxStringLen macro in NSString.cpp file.
     *  @return A NSString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */ 
    static NSString* createWithFormat(const char* format, ...);

    /** create a string with binary data 
     *  @return A NSString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static NSString* createWithData(const unsigned char* pData, unsigned long nLen);

    /** create a string with a file, 
     *  @return A NSString pointer which is an autorelease object pointer,
     *          it means that you needn't do a release operation unless you retain it.
     */
    static NSString* createWithContentsOfFile(const char* pszFileName);

private:

    /** only for internal use */
    bool initWithFormatAndValist(const char* format, va_list ap);

public:
    std::string m_sString;
};

struct NSStringCompare : public std::binary_function<NSString *, NSString *, bool> {
    public:
        bool operator() (NSString * a, NSString * b) const {
            return strcmp(a->getCString(), b->getCString()) < 0;
        }
};

#define NSStringMake(str) NSString::create(str)
#define nss               NSStringMake

// end of data_structure group
/// @}

NS_NS_END

#endif //__NSSTRING_H__
