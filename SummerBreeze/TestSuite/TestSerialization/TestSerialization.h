#ifndef __TEST_SERIALIZATION_CPP_H__
#define __TEST_SERIALIZATION_CPP_H__

#include "../../SharedLib/Shared/Serialization/PackPacket.h"
#include "../../SharedLib/Shared/Serialization/UnpackPacket.h"


class OpcodeBase
{
public:
	OpcodeBase(){}
	virtual ~OpcodeBase(){}

public:
	virtual void Load(UnpackPacket& stream) = 0;
	virtual void Save(PackPacket& stream) = 0;
	virtual void Reset(void) = 0;
};

extern UnpackPacket& operator >> (UnpackPacket& stream, OpcodeBase* opcode);
extern PackPacket& operator << (PackPacket& stream, OpcodeBase* opcode);


class OpcodeBase_1 : public OpcodeBase
{
public:
	OpcodeBase_1(void)
	{
		this->value_1 = 1;
		this->value_2 = 2;
		this->value_3 = 3;
		this->value_4 = 4;
	}

	virtual void Load(UnpackPacket& stream)
	{
		stream >> value_1;
		stream >> value_2;
		stream >> value_3;
		stream >> value_4;
	}

	virtual void Save(PackPacket& stream)
	{
		stream << value_1;
		stream << value_2;
		stream << value_3;
		stream << value_4;
	}

	virtual void Reset()
	{
		this->value_1 = 0;
		this->value_2 = 0;
		this->value_3 = 0;
		this->value_4 = 0;
	}

public:
	int8_t value_1;
	int16_t value_2;
	int32_t value_3;
	int64_t value_4;
};

class OpcodeBase_2 : public OpcodeBase
{
public:
	OpcodeBase_2(void)
	{
		this->value_1 = 11;
		this->value_2 = 21;
		this->value_3 = 31;
		this->value_4 = 41;
	}

	virtual void Load(UnpackPacket& stream)
	{
		stream >> value_1;
		stream >> value_2;
		stream >> value_3;
		stream >> value_4;
	}

	virtual void Save(PackPacket& stream)
	{
		stream << value_1;
		stream << value_2;
		stream << value_3;
		stream << value_4;
	}

	virtual void Reset()
	{
		this->value_1 = 0;
		this->value_2 = 0;
		this->value_3 = 0;
		this->value_4 = 0;
	}

public:
	int8_t value_1;
	int16_t value_2;
	int32_t value_3;
	int64_t value_4;
};

class TestSerialization
{
public:
	TestSerialization(void);
	~TestSerialization(void);

public:
	int Run(void);
};

#endif