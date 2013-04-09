#include "simple_class.h"

SimpleNativeClass::SimpleNativeClass() { m_someField = 6; };

// these methods are simple, can be defined inline
int SimpleNativeClass::getSomeField() {
	return m_someField;
}

void SimpleNativeClass::log(const char* message) {
	printf("%s\n", message);
}