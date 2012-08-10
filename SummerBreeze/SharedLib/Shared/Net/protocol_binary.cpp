#include "protocol_binary.hpp"

#include "../Serialization/PackPacket.h"
#include "../Serialization/UnpackPacket.h"


UnpackPacket& operator >> (UnpackPacket& stream, protocol_binary_t* opcode)
{
	opcode->load(stream);
	return stream;
}

PackPacket& operator << (PackPacket& stream, protocol_binary_t* opcode)
{
	opcode->save(stream);
	return stream;
}