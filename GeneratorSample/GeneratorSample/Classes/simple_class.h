#ifndef __SIMPLE_CLASS_H__
#define __SIMPLE_CLASS_H__

#include <string>
#include <stdint.h>

class SimpleNativeClass
{
protected:
	int m_someField;

public:

	SimpleNativeClass();

	// these methods are simple, can be defined inline
	int getSomeField();

	void log(const char* message);
};

#endif
