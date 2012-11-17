#include "protocol_binary.hpp"

#include "../Serialization/PackPacket.h"
#include "../Serialization/UnpackPacket.h"

#include "concrete_protocol.hpp"

std::map<uint32_t, protocol_binary_t*> protocol_binary_t::prototypes_map;
std::map<uint32_t, std::set<zmq::fd_t> > protocol_binary_t::s_route_map;
std::set<uint32_t> protocol_binary_t::s_register_opcode;

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
	 stream >> v.origin_session;
	 stream >> v.session;
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
	 stream << v.origin_session;
	 stream << v.session;
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
	 if (!ret.second)
	 {
		 std::stringstream ss;
		 ss << "add_prototype error : " << __FILE__ << " " << __LINE__ ; 
		 std::cout << ss.str();
		 throw std::exception(ss.str().c_str());
	 }
 }

 void protocol_binary_t::add_prototype_instance(void)
 {
	 add_prototype(&protocol_test_t::s_protocol_test);
	 add_prototype(&protocol_mount_t::s_protocol_mount);
 }

 bool protocol_binary_t::find_route(uint32_t opcode, std::set<zmq::fd_t> &ref)
 {
	 std::map<uint32_t, std::set<zmq::fd_t> >::iterator ite = s_route_map.find(opcode);
	 if (ite != s_route_map.end())
	 {
		 ref = ite->second;
		 return true;
	 } 
	 else
	 {
		 return false;
	 }
 }

 void protocol_binary_t::handle_mount(zmq::fd_t fds, std::vector<uint32_t> opcode_vec)
 {
	 for (std::vector<uint32_t>::iterator ite = opcode_vec.begin();
		 ite != opcode_vec.end();
		 ite++)
	 {
		 std::map<uint32_t, std::set<zmq::fd_t> >::iterator set_ite = s_route_map.find(*ite);
		 if (set_ite != s_route_map.end())
		 {
			 set_ite->second.insert(*ite);

			 //if (set_ite->second.size() > 1)
			 {
				 std::cout << __FILE__ << ":" << __LINE__ << " "<< "opcode fd_t size " << set_ite->second.size() << std::endl;
			 }
		 } 
		 else
		 {
			 std::set<zmq::fd_t> fds_set;
			 fds_set.insert(*ite);

			 s_route_map[*ite] = fds_set;
		 }
	 }
 }

  bool protocol_binary_t::registered(uint32_t opcode_)
  {
	 return s_register_opcode.count(opcode_) > 0 ? true : false;
  }

  void protocol_binary_t::add_registry(std::set<uint32_t> opcode_set)
  {
	  for (std::set<uint32_t>::iterator ite = opcode_set.begin();
		  ite != opcode_set.end();
		  ite++)
	  {
		  s_register_opcode.insert(*ite);
	  }
  }