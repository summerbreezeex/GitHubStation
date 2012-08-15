#include "TestNetContext.h"

#include "../../SharedLib/Shared/Net/zmq_listener.hpp"
#include "../../SharedLib/Shared/Net/concrete_protocol.hpp"
using namespace zmq;

#include <iostream>
#include <sstream>

TestNetContext::TestNetContext(void)
{

}

TestNetContext::~TestNetContext(void)
{

}
int TestNetContext::Run(void)
{
	ctx_t context(200);
	int process_id = GetCurrentProcessId();
	int server_port = 5000 + process_id;
	context.acceptor()->set_address(server_port, "127.0.0.1");
	context.acceptor()->process_plug();

	zmq_sleep(10000);

	//context.connect_address(4567, "127.0.0.1", "client_1");
	//context.connect_address(4567, "127.0.0.1", "client_2");
	//context.connect_address(4567, "127.0.0.1", "client_3");
	//context.connect_address(4567, "127.0.0.1", "client_4");
	//context.connect_address(4567, "127.0.0.1", "client_5");
	//context.connect_address(4567, "127.0.0.1", "client_6");
	//context.connect_address(4567, "127.0.0.1", "client_7");
	//context.connect_address(4567, "127.0.0.1", "client_8");
	//context.connect_address(4567, "127.0.0.1", "client_9");
	//context.connect_address(4567, "127.0.0.1", "client_10");

	//for (std::map<std::string, fd_t>::iterator ite = context.get_active_connnect().begin();
	//	ite != context.get_active_connnect().end();
	//	ite++)
	//{
	//	std::stringstream ss;
	//	ss << "*" << ite->first << "*";


	//	protocol_test_t * ptr_protocol = new protocol_test_t(); 
	//	ptr_protocol->data = ss.str();

	//	PackPacket pack_packet;
	//	pack_packet << ptr_protocol;

	//	std::cout << "send : " << ptr_protocol->data << std::endl;

	//	int pack_length = pack_packet.Length();
	//	int nSend = ::send(ite->second, pack_packet.Buf(), pack_length, 0);
	//}

	int count = 0;
	while(count < 200)
	{
		count++;
		std::stringstream ss;
		ss << count ;

		context.connect_address(9999, "127.0.0.1", ss.str());
		std::map<std::string, fd_t>::iterator ite = context.get_active_connnect().find(ss.str());
		if (ite != context.get_active_connnect().end())
		{
			
			ss << "*" << count << "*";


			protocol_test_t * ptr_protocol = new protocol_test_t(); 
			ptr_protocol->data = ss.str();

			PackPacket pack_packet;
			pack_packet << ptr_protocol;

			std::cout << "send : " << ptr_protocol->data << std::endl;

			int pack_length = pack_packet.Length();
			int nSend = ::send(ite->second, pack_packet.Buf(), pack_length, 0);
		}
	}

	system("pause");

	return 0;
}