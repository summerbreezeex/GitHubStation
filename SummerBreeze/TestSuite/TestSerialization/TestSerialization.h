#ifndef __TEST_SERIALIZATION_CPP_H__
#define __TEST_SERIALIZATION_CPP_H__

#include "../../SharedLib/Shared/Serialization/PackPacket.h"
#include "../../SharedLib/Shared/Serialization/UnpackPacket.h"

class TestSerialization
{
public:
	TestSerialization(void);
	~TestSerialization(void);

public:
	int Run(void);
};

#endif