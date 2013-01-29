#ifndef __NSINTEGER_H__
#define __NSINTEGER_H__

#include "NSObject.h"

NS_NS_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

class NS_DLL NSInteger : public NSObject
{
public:
    NSInteger(int v)
        : m_nValue(v) {}
    int getValue() const {return m_nValue;}

    // @deprecated: This interface will be deprecated sooner or later.
    NS_DEPRECATED_ATTRIBUTE static NSInteger* integerWithInt(int v)
    {
        return NSInteger::create(v);
    }

    static NSInteger* create(int v)
    {
        NSInteger* pRet = new NSInteger(v);
        pRet->autorelease();
        return pRet;
    }
private:
    int m_nValue;
};

// end of data_structure group
/// @}

NS_NS_END

#endif /* __NSINTEGER_H__ */
