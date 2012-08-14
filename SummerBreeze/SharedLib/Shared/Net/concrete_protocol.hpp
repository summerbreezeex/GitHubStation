#ifndef __ZMQ_CONCRETE_PROTOCOL_HPP_INCLUDED__
#define __ZMQ_CONCRETE_PROTOCOL_HPP_INCLUDED__

#include <string>

#include "protocol_binary.hpp"
#include "fd.hpp"

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
		stream >> data;
	}

	virtual void save(PackPacket& stream)
	{
		stream << data;
	}
	virtual void run(void)
	{
		std::cout << "session : " << this->head.session << " data : " << data << std::endl;

		protocol_test_t * ptr_protocol = new protocol_test_t(); 
		ptr_protocol->data = this->data;

		PackPacket pack_packet;
		pack_packet << ptr_protocol;
		
		delete ptr_protocol;
		ptr_protocol = NULL;

		int nSend = ::send(this->head.session, pack_packet.Buf(), pack_packet.Length(), 0);
	}

public:
	std::string data;

public:
	static protocol_test_t s_protocol_test;
};

#endif