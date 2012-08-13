#ifndef __ZMQ_CONCRETE_PROTOCOL_HPP_INCLUDED__
#define __ZMQ_CONCRETE_PROTOCOL_HPP_INCLUDED__

#include "protocol_binary.hpp"

class protocol_test_t : public protocol_binary_t
{
public:
	protocol_test_t()
	{
		this->head.opcode = 1;
	}

	protocol_binary_t * clone()
	{
		return new protocol_test_t();
	}

public:
	virtual void load(UnpackPacket& stream)
	{

	}

	virtual void save(PackPacket& stream)
	{

	}
	virtual void run(void)
	{
		std::cout << "protocol_test_t" << std::endl;
	}

public:
	static protocol_test_t s_protocol_test;
};

#endif