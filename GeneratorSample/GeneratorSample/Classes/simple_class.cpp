#include "simple_class.h"

SimpleNativeClass::SimpleNativeClass() { m_someField = 11111; };

// these methods are simple, can be defined inline
int SimpleNativeClass::getSomeField() {
	return m_someField;
}

void SimpleNativeClass::print(const char* message) {
	printf("%s\n", message);
}