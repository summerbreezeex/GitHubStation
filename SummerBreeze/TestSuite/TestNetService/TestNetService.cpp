#include "TestNetService.h"


#include "../../SharedLib/Shared/Net/zmq_listener.hpp"
using namespace zmq;

TestNetService::TestNetService(void)
{

}

TestNetService::~TestNetService(void)
{

}
int TestNetService::Run(void)
{
	ctx_t context(100);

	//sockaddr_in servce_addr;

	//// Set up the sockaddr structure
	//servce_addr.sin_family = AF_INET;
	//servce_addr.sin_port = htons(4567);
	//servce_addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);

	context.acceptor()->set_address(4567, "127.0.0.1");
	context.acceptor()->process_plug();

	system("pause");

	return 0;
}