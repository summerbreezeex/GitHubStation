#include "protocol_binary.hpp"

#include "../Serialization/PackPacket.h"
#include "../Serialization/UnpackPacket.h"

std::map<uint32_t, protocol_binary_t*> protocol_binary_t::prototypes_map;

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
	stream >> v.options_opcode;
	stream >> v.options_values_length;
	stream >> v.options_values;

	return stream;
}

PackPacket &operator<<(PackPacket& stream, protocol_binary_t::head_options_t v)
{
	stream << v.options_opcode;
	stream << v.options_values_length;
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
	 int temp_1 = sizeof(binary_head_t);
	 int tmep_2 = sizeof(std::vector<head_options_t>);
	 if (head_len < temp_1 - tmep_2)
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

 protocol_binary_t* protocol_binary_t::find_and_clone(uint32_t opcode_)
 {
	 std::map<uint32_t, protocol_binary_t*>::iterator ite = prototypes_map.find(opcode_);
	 if (ite != prototypes_map.end())
	 {
		 if (ite->second->head.opcode == opcode_)
		 {
			 return ite->second->clone();
		 }
		 else
		 {
			 std::stringstream ss;
			 ss << "find_and_clone match error : " << __FILE__ << " " << __LINE__ ; 
			 std::cout << ss.str();
			 throw std::exception(ss.str().c_str());
		 }

	 }
	 else
	 {
		 std::stringstream ss;
		 ss << "find_and_clone not find error : " << __FILE__ << " " << __LINE__ ; 
		 std::cout << ss.str();
		 throw std::exception(ss.str().c_str());
	 }

	 return NULL;
 }

 void protocol_binary_t::add_prototype(protocol_binary_t *protocal)
 {
	 std::pair<std::map<uint32_t, protocol_binary_t*>::iterator,bool> ret;
	 ret = prototypes_map.insert(std::pair<uint32_t, protocol_binary_t*>(protocal->head.opcode, protocal));
	 //if (!ret.second)
	 //{
		// std::stringstream ss;
		// ss << "add_prototype error : " << __FILE__ << " " << __LINE__ ; 
		// std::cout << ss.str();
		// throw std::exception(ss.str().c_str());
	 //}
 }