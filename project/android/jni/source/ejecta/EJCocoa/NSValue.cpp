/****************************************************************************

 ****************************************************************************/
#include "NSValue.h"

NS_NS_BEGIN

NSValue::NSValue() : m_value(0), m_type(kJSClassRef) {

}

NSValue::NSValue(void* value, NSValueType type) : m_value(value), m_type(type) {

}

NSValue::~NSValue() {
	releaseValue();
}


void NSValue::releaseValue() {
	if (m_value) {
		switch(m_type) {
			case kJSClassRef:
				JSClassRelease((JSClassRef)m_value);
				break;
			case kJSObjectRef:
				m_value = 0;
				break;
			default:
				break;
		}
	}
}

void NSValue::setValue(void* value, NSValueType type) {
	releaseValue();
	m_value = value;
	m_type = type;
}

void* NSValue::pointerValue() {
	return m_value;
}

NS_NS_END
