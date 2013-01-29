/****************************************************************************

 ****************************************************************************/

#ifndef __NS_VALUE_H__
#define __NS_VALUE_H__

#include <JavaScriptCore/JavaScriptCore.h>

#include "support/NSPlatformMacros.h"
#include "NSObject.h"

NS_NS_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

typedef enum {
	kJSClassRef,
	kJSObjectRef
} NSValueType;

class NSValue: public NSObject {
	NSValueType m_type;
	void* m_value;
	void releaseValue();
public:
	NSValue();
	NSValue(void* value, NSValueType type);
	virtual ~NSValue();

	void setValue(void* value, NSValueType type);
	void* pointerValue();
};

// end of data_structure group
/// @}

NS_NS_END

#endif // __NS_VALUE_H__
