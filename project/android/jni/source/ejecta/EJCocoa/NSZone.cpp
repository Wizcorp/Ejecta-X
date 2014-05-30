/****************************************************************************

****************************************************************************/
#include "NSZone.h"
#include "NSObject.h"

NS_NS_BEGIN

NSZone::NSZone(NSObject *pObject)
{
    m_pCopyObject = pObject;
}

NS_NS_END
