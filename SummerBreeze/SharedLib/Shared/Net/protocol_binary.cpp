#include "protocol_binary.hpp"

#include "../Serialization/PackPacket.h"
#include "../Serialization/UnpackPacket.h"


UnpackPacket& operator >> (UnpackPacket& stream, protocol_binary_t* opcode)
{
	opcode->unpack(stream);
	return stream;
}

PackPacket& operator << (PackPacket& stream, protocol_binary_t* opcode)
{
	opcode->pack(stream);
	return stream;
}

 UnpackPacket &operator>>(UnpackPacket& stream, protocol_binary_t::head_options_t& v)
 {
	 stream >> v.options_length;
	 stream >> v.options_opcode;
	 stream >> v.options_values;

	 return stream;
 }

 PackPacket &operator<<(PackPacket& stream, protocol_binary_t::head_options_t v)
 {
	 stream << v.options_length;
	 stream << v.options_opcode;
	 stream << v.options_values;
	 
	 return stream;
 }

 UnpackPacket &operator>>(UnpackPacket& stream, protocol_binary_t::binary_head_t& v)
 {
	 stream >> v.version;
	 stream >> v.head_length;
	 stream >> v.type_of_service;
	 stream >> v.total_length;
	 stream >> v.identification;
	 stream >> v.opcode;
	 stream >> v.options_vector;

	 return stream;
 }

 PackPacket &operator<<(PackPacket& stream, protocol_binary_t::binary_head_t v)
 {
	 stream << v.version;
	 stream << v.head_length;
	 stream << v.type_of_service;
	 stream << v.total_length;
	 stream << v.identification;
	 stream << v.opcode;
	 stream << v.options_vector;

	 return stream;
 }

 void protocol_binary_t::pack(PackPacket& stream)
 {
	 int old_length = stream.Length();

	 stream << head;

	 int head_len = stream.Length() - old_length;
	 if (head_len < sizeof(binary_head_t) - sizeof(std::vector<head_options_t>))
	 {
		 throw std::exception("protocol_binary_t");
	 } 
	 else
	 {
		 ((binary_head_t*)(stream.Buf() + old_length))->head_length = head_len;
		 save(stream);

		 uint32_t total_len = stream.Length() - old_length;

		 ((binary_head_t*)(stream.Buf() + old_length))->total_length = total_len;
	 }
 }

 void protocol_binary_t::unpack(UnpackPacket& stream)
 {
	 stream >> head;
	 load(stream);
 }
