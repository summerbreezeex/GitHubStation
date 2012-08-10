#include "TestSerialization.h"

#include <string>
#include <vector>

UnpackPacket& operator >> (UnpackPacket& stream, OpcodeBase* opcode)
{
	opcode->Load(stream);
	return stream;
}

PackPacket& operator << (PackPacket& stream, OpcodeBase* opcode)
{
	opcode->Save(stream);
	return stream;
}

TestSerialization::TestSerialization(void)
{

}

TestSerialization::~TestSerialization(void)
{

}

int TestSerialization::Run(void)
{
	int8_t value_1 = 1;
	int16_t value_2 = 2;
	int32_t value_3 = 3;
	int64_t value_4 = 4;

	uint8_t value_5 = 5;
	uint16_t value_6 = 6;
	uint32_t value_7 = 7;
	uint64_t value_8 = 8;

	float value_9 = 9.9;
	double value_10 = 10.1;

	bool value_11 = true;
	std::string value_12 = "Hello Wolrd!";

	std::vector<uint32_t> value_13;
	value_13.push_back(13);
	value_13.push_back(13);
	value_13.push_back(13);
	value_13.push_back(13);
	value_13.push_back(13);

	std::vector<std::string> value_14;
	value_14.push_back("Hello");
	value_14.push_back("Hello");
	value_14.push_back("Hello");

	uint32_t value_15 = 15;


	std::cout << value_1 << std::endl;
	std::cout << value_2 << std::endl;
	std::cout << value_3 << std::endl;
	std::cout << value_4 << std::endl;
	std::cout << value_5 << std::endl;
	std::cout << value_6 << std::endl;
	std::cout << value_7 << std::endl;
	std::cout << value_8 << std::endl;
	std::cout << value_9 << std::endl;
	std::cout << value_10 << std::endl;
	std::cout << value_11 << std::endl;
	std::cout << value_12 << std::endl;
	std::cout << value_15 << std::endl;

	PackPacket pack_packet;
	pack_packet << value_1;
	pack_packet << value_2;
	pack_packet << value_3;
	pack_packet << value_4;
	pack_packet << value_5;
	pack_packet << value_6;
	pack_packet << value_7;
	pack_packet << value_8;
	pack_packet << value_9;
	pack_packet << value_10;
	pack_packet << value_11;
	pack_packet << value_12;
	pack_packet << value_13;
	pack_packet << value_14;
	pack_packet << value_15;


	value_1 = 0;
	value_2 = 0;
	value_3 = 0;
	value_4 = 0;
	value_5 = 0;
	value_6 = 0;
	value_7 = 0;
	value_8 = 0;
	value_9 = 0;
	value_10 = 0;
	value_11 = 0;
	value_12 = "";
	value_13.clear();
	value_14.clear();
	value_15 = 0;

	std::cout << std::endl;

	std::cout << value_1 << std::endl;
	std::cout << value_2 << std::endl;
	std::cout << value_3 << std::endl;
	std::cout << value_4 << std::endl;
	std::cout << value_5 << std::endl;
	std::cout << value_6 << std::endl;
	std::cout << value_7 << std::endl;
	std::cout << value_8 << std::endl;
	std::cout << value_9 << std::endl;
	std::cout << value_10 << std::endl;
	std::cout << value_11 << std::endl;
	std::cout << value_12 << std::endl;
	std::cout << value_15 << std::endl;

	UnpackPacket unpack_packet(pack_packet.Buf(), pack_packet.Length());
	unpack_packet >> value_1;
	unpack_packet >> value_2;
	unpack_packet >> value_3;
	unpack_packet >> value_4;
	unpack_packet >> value_5;
	unpack_packet >> value_6;
	unpack_packet >> value_7;
	unpack_packet >> value_8;
	unpack_packet >> value_9;
	unpack_packet >> value_10;
	unpack_packet >> value_11;
	unpack_packet >> value_12;
	unpack_packet >> value_13;
	unpack_packet >> value_14;
	unpack_packet >> value_15;

	std::cout << std::endl;

	std::cout << value_1 << std::endl;
	std::cout << value_2 << std::endl;
	std::cout << value_3 << std::endl;
	std::cout << value_4 << std::endl;
	std::cout << value_5 << std::endl;
	std::cout << value_6 << std::endl;
	std::cout << value_7 << std::endl;
	std::cout << value_8 << std::endl;
	std::cout << value_9 << std::endl;
	std::cout << value_10 << std::endl;
	std::cout << value_11 << std::endl;
	std::cout << value_12 << std::endl;
	std::cout << value_15 << std::endl;


	OpcodeBase *ptr_opcode_1 = new OpcodeBase_1();
	OpcodeBase *ptr_opcode_2 = new OpcodeBase_2();

	PackPacket pack_packet_1;
	pack_packet_1 << ptr_opcode_1;
	pack_packet_1 << ptr_opcode_2;

	ptr_opcode_1->Reset();
	ptr_opcode_2->Reset();

	UnpackPacket unpack_packet_1(pack_packet_1.Buf(), pack_packet_1.Length());

	unpack_packet_1 >> ptr_opcode_1;
	unpack_packet_1 >> ptr_opcode_2;

	return 1;
}