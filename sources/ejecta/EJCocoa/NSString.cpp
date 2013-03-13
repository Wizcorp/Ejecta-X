#include "NSString.h"
//#include "NSFileUtils.h"
#include "support/nsMacros.h"
#include <stdlib.h>
#include <stdio.h>

NS_NS_BEGIN

#define kMaxStringLen (1024*100)

NSString::NSString()
    :m_sString("")
{}

NSString::NSString(const char * str)
    :m_sString(str)
{}

NSString::NSString(const std::string& str)
    :m_sString(str)
{}

NSString::NSString(const NSString& str)
    :m_sString(str.getCString())
{}

NSString::~NSString()
{ 
    m_sString.clear();
}

NSString& NSString::operator= (const NSString& other)
{
    m_sString = other.m_sString;
    return *this;
}

bool NSString::initWithFormatAndValist(const char* format, va_list ap)
{
    bool bRet = false;
    char* pBuf = (char*)malloc(kMaxStringLen);
    if (pBuf != NULL)
    {
        vsnprintf(pBuf, kMaxStringLen, format, ap);
        m_sString = pBuf;
        free(pBuf);
        bRet = true;
    }
    return bRet;
}

bool NSString::initWithFormat(const char* format, ...)
{
    bool bRet = false;
    m_sString.clear();

    va_list ap;
    va_start(ap, format);

    bRet = initWithFormatAndValist(format, ap);

    va_end(ap);

    return bRet;
}

int NSString::intValue() const
{
    if (length() == 0)
    {
        return 0;
    }
    return atoi(m_sString.c_str());
}

unsigned int NSString::uintValue() const
{
    if (length() == 0)
    {
        return 0;
    }
    return (unsigned int)atoi(m_sString.c_str());
}

float NSString::floatValue() const
{
    if (length() == 0)
    {
        return 0.0f;
    }
    return (float)atof(m_sString.c_str());
}

double NSString::doubleValue() const
{
    if (length() == 0)
    {
        return 0.0;
    }
    return atof(m_sString.c_str());
}

bool NSString::boolValue() const
{
    if (length() == 0)
    {
        return false;
    }

    if (0 == strcmp(m_sString.c_str(), "0") || 0 == strcmp(m_sString.c_str(), "false"))
    {
        return false;
    }
    return true;
}

const char* NSString::getCString() const
{
    return m_sString.c_str();
}


const char* NSString::pathExtension()const
{
    int pos = m_sString.find_last_of(".");
    if (pos == std::string::npos)
    {
        return "";
    }
    else
    {
        return (m_sString.substr(pos + 1)).c_str();
    }
}

unsigned int NSString::length() const
{
    return m_sString.length();
}

int NSString::compare(const char * pStr) const
{
    return strcmp(getCString(), pStr);
}

NSObject* NSString::copyWithZone(NSZone* pZone)
{
    NSAssert(pZone == NULL, "NSString should not be inherited.");
    NSString* pStr = new NSString(m_sString.c_str());
    return pStr;
}

bool NSString::isEqual(const NSObject* pObject)
{
    bool bRet = false;
    const NSString* pStr = dynamic_cast<const NSString*>(pObject);
    if (pStr != NULL)
    {
        if (0 == m_sString.compare(pStr->m_sString))
        {
            bRet = true;
        }
    }
    return bRet;
}

NSString* NSString::stringWithCString(const char* pStr)
{
    return NSString::create(pStr);
}

NSString* NSString::create(const std::string& str)
{
    NSString* pRet = new NSString(str);
    pRet->autorelease();
    return pRet;
}

NSString* NSString::stringWithString(const std::string& pStr)
{
    NSString* pRet = new NSString(pStr);
    pRet->autorelease();
    return pRet;
}

NSString* NSString::stringWithData(const unsigned char* pData, unsigned long nLen)
{
    return NSString::createWithData(pData, nLen);
}

NSString* NSString::createWithData(const unsigned char* pData, unsigned long nLen)
{
    NSString* pRet = NULL;
    if (pData != NULL)
    {
        char* pStr = (char*)malloc(nLen+1);
        if (pStr != NULL)
        {
            pStr[nLen] = '\0';
            if (nLen > 0)
            {
                memcpy(pStr, pData, nLen);
            }
            
            pRet = NSString::create(pStr);
            free(pStr);
        }
    }
    return pRet;
}

NSString* NSString::stringWithFormat(const char* format, ...)
{
    NSString* pRet = NSString::create("");
    va_list ap;
    va_start(ap, format);
    pRet->initWithFormatAndValist(format, ap);
    va_end(ap);

    return pRet;
}

NSString* NSString::createWithFormat(const char* format, ...)
{
    NSString* pRet = NSString::create("");
    va_list ap;
    va_start(ap, format);
    pRet->initWithFormatAndValist(format, ap);
    va_end(ap);

    return pRet;
}

NSString* NSString::stringWithContentsOfFile(const char* pszFileName)
{
    return NSString::createWithContentsOfFile(pszFileName);
}

unsigned char* getFileData(const char* fileName, const char* pszMode, unsigned long * pSize)
{
    unsigned char* buffer;
    
    size_t buffer_size = 0;
    size_t buffer_capacity = 1024;
    buffer = (unsigned char*)malloc(buffer_capacity);
    
    FILE* f = fopen(fileName, pszMode);
    if (!f) {
        //NSLOG("Could not open file: %s\n", fileName);
        return 0;
    }
    
    while (!feof(f) && !ferror(f)) {
        buffer_size += fread(buffer + buffer_size, 1, buffer_capacity - buffer_size, f);
        if (buffer_size == buffer_capacity) { /* guarantees space for trailing '\0' */
            buffer_capacity *= 2;
            buffer = (unsigned char*)realloc(buffer, buffer_capacity);
            NSAssert(buffer != NULL, "NSString getFileData null buffer.");
        }
        
        NSAssert(buffer_size < buffer_capacity, "NSString getFileData error size.");
    }
    fclose(f);
    buffer[buffer_size] = '\0';  

    if (pSize)
    {
        *pSize = buffer_size;
    }
    
    return buffer;
}

NSString* NSString::createWithContentsOfFile(const char* pszFileName)
{
    unsigned long size = 0;
    unsigned char* pData = 0;
    NSString* pRet = NULL;
    pData = getFileData(pszFileName, "rb", &size);
    pRet = NSString::createWithData(pData, size);
    NS_SAFE_DELETE_ARRAY(pData);
    return pRet;
}

NS_NS_END
