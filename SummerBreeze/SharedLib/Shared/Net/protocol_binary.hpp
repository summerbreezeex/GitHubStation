#ifndef __ZMQ_PTOTOCOL_BINARY_HPP_INCLUDED__
#define __ZMQ_PTOTOCOL_BINARY_HPP_INCLUDED__

#include "../Serialization/PackPacket.h"
#include "../Serialization/UnpackPacket.h"

class protocol_binary_t
{
public:
	protocol_binary_t(){}
	virtual ~protocol_binary_t(){}

public:
	virtual void load(UnpackPacket& stream) = 0;
	virtual void save(PackPacket& stream) = 0;
	virtual void run(void) = 0;

private:
	protocol_binary_t (const protocol_binary_t&);
	const protocol_binary_t &operator = (const protocol_binary_t&);
};

extern UnpackPacket& operator >> (UnpackPacket& stream, protocol_binary_t* opcode);
extern PackPacket& operator << (PackPacket& stream, protocol_binary_t* opcode);

#endif