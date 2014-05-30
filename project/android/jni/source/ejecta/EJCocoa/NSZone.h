/****************************************************************************

****************************************************************************/

#ifndef __NS_ZONE_H__
#define __NS_ZONE_H__

#include "support/NSPlatformMacros.h"
#include <stdio.h>

NS_NS_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

class NSObject;

class NS_DLL NSZone
{
public:
    NSZone(NSObject *pObject = NULL);

public:
    NSObject *m_pCopyObject;
};

// end of data_structure group
/// @}

NS_NS_END

#endif // __NS_ZONE_H__

