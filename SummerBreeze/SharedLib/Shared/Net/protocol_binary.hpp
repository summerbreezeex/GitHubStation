#ifndef __ZMQ_PTOTOCOL_BINARY_HPP_INCLUDED__
#define __ZMQ_PTOTOCOL_BINARY_HPP_INCLUDED__

#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <new>
#include <exception>

#include "../Serialization/PackPacket.h"
#include "../Serialization/UnpackPacket.h"

#include "fd.hpp"

class protocol_binary_t
{

public:
	protocol_binary_t(){}
	virtual ~protocol_binary_t(){}

public:
	//Template Method
	void pack(PackPacket& stream);
	void unpack(UnpackPacket& stream);

public:
	virtual void load(UnpackPacket& stream) = 0;
	virtual void save(PackPacket& stream) = 0;
	virtual void run(void) = 0;

public:

#pragma pack(1)
	struct head_options_t
	{
		head_options_t()
		{
			this->options_opcode = 0;
			this->options_values_length = 0;
		}

		uint16_t options_opcode;
		uint16_t options_values_length;
		std::vector<uint8_t> options_values;
	};

	struct binary_head_t
	{
		binary_head_t()
		{
			this->version = 0;
			this->head_length = 0;
			this->type_of_service = 0;
			this->total_length = 0;
			this->identification = 0;
			this->opcode = 0;
			this->origin_session = 0;
			this->session = 0;
		}
		uint8_t version;
		uint16_t head_length;
		uint8_t type_of_service;
		uint32_t total_length;
		uint32_t identification;
		uint32_t opcode;
		uint32_t origin_session;
		zmq::fd_t session;

		std::vector<head_options_t> options_vector;
	};
#pragma pack()

	binary_head_t head;

public:
	static protocol_binary_t *find_and_clone(uint32_t opcode_);
	static void add_prototype_instance(void);

	static bool find_route(uint32_t opcode, std::set<zmq::fd_t> &ref);
	static void handle_mount(zmq::fd_t fds, std::vector<uint32_t> opcode_vec);

	static bool registered(uint32_t opcode_);
	static void add_registry(std::set<uint32_t> opcode_vec);

protected:
	virtual protocol_binary_t *clone() = 0;
	// As each subclass of Image is declared, it registers its prototype
	static void add_prototype(protocol_binary_t *protocal);

private:
	static std::map<uint32_t, protocol_binary_t*> prototypes_map;
	//key:操作码
	static std::map<uint32_t, std::set<zmq::fd_t> > s_route_map;

	static std::set<uint32_t> s_register_opcode;

private:
	protocol_binary_t (const protocol_binary_t&);
	const protocol_binary_t &operator = (const protocol_binary_t&);
};

extern UnpackPacket& operator >> (UnpackPacket& stream, protocol_binary_t* opcode);
extern PackPacket& operator << (PackPacket& stream, protocol_binary_t* opcode);

extern UnpackPacket &operator>>(UnpackPacket& stream, protocol_binary_t::head_options_t& v);
extern PackPacket &operator<<(PackPacket& stream, protocol_binary_t::head_options_t v);

extern UnpackPacket &operator>>(UnpackPacket& stream, protocol_binary_t::binary_head_t& v);
extern PackPacket &operator<<(PackPacket& stream, protocol_binary_t::binary_head_t v);

/*
www.ietf.org/rfc/rfc791.txt

协议格式以IP协议为参考

3.1.  Internet Header Format

A summary of the contents of the internet header follows:


0                   1                   2                   3   
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Version|  IHL  |Type of Service|          Total Length         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Identification        |Flags|      Fragment Offset    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Time to Live |    Protocol   |         Header Checksum       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                       Source Address                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Destination Address                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                    Options                    |    Padding    |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Example Internet Datagram Header

Figure 4.


Notation:

FO    -  Fragment Offset
IHL   -  Internet Header Length
DF    -  Don't Fragment flag
MF    -  More Fragments flag
TL    -  Total Length
OFO   -  Old Fragment Offset
OIHL  -  Old Internet Header Length
OMF   -  Old More Fragments flag
OTL   -  Old Total Length
NFB   -  Number of Fragment Blocks

MTU   -  Maximum Transmission Unit

Notation:

FO    -  Fragment Offset
IHL   -  Internet Header Length
MF    -  More Fragments flag
TTL   -  Time To Live
NFB   -  Number of Fragment Blocks
TL    -  Total Length
TDL   -  Total Data Length
BUFID -  Buffer Identifier
RCVBT -  Fragment Received Bit Table
TLB   -  Timer Lower Bound


Example 1:

This is an example of the minimal data carrying internet datagram:


0                   1                   2                   3   
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Ver= 4 |IHL= 5 |Type of Service|        Total Length = 21      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|      Identification = 111     |Flg=0|   Fragment Offset = 0   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   Time = 123  |  Protocol = 1 |        header checksum        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         source address                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                      destination address                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     data      |                                                
+-+-+-+-+-+-+-+-+                                                

Example Internet Datagram

Figure 5.

Note that each tick mark represents one bit position.

This is a internet datagram in version 4 of internet protocol; the
internet header consists of five 32 bit words, and the total length of
the datagram is 21 octets.  This datagram is a complete datagram (not
a fragment).



Example 2:

In this example, we show first a moderate size internet datagram (452
data octets), then two internet fragments that might result from the
fragmentation of this datagram if the maximum sized transmission
allowed were 280 octets.


0                   1                   2                   3   
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Ver= 4 |IHL= 5 |Type of Service|       Total Length = 472      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Identification = 111      |Flg=0|     Fragment Offset = 0 |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   Time = 123  | Protocol = 6  |        header checksum        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         source address                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                      destination address                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             data                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             data                              |
\                                                               \
\                                                               \
|                             data                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             data              |                                
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                

Example Internet Datagram

Figure 6.


September 1981                                                          
Internet Protocol



And the second fragment.


0                   1                   2                   3   
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Ver= 4 |IHL= 5 |Type of Service|       Total Length = 216      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Identification = 111      |Flg=0|  Fragment Offset  =  32 |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   Time = 119  | Protocol = 6  |        Header Checksum        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         source address                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                      destination address                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             data                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             data                              |
\                                                               \
\                                                               \
|                             data                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            data               |                                
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                

Example Internet Fragment

Figure 8.


Example 3:

Here, we show an example of a datagram containing options:


0                   1                   2                   3   
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|Ver= 4 |IHL= 8 |Type of Service|       Total Length = 576      |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|       Identification = 111    |Flg=0|     Fragment Offset = 0 |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|   Time = 123  |  Protocol = 6 |       Header Checksum         |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                        source address                         |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                      destination address                      |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| Opt. Code = x | Opt.  Len.= 3 | option value  | Opt. Code = x |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| Opt. Len. = 4 |           option value        | Opt. Code = 1 |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	| Opt. Code = y | Opt. Len. = 3 |  option value | Opt. Code = 0 |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                             data                              |
	\                                                               \
	\                                                               \
	|                             data                              |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                             data                              |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	Example Internet Datagram

	Figure 9.
*/

#endif