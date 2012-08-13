#include <iostream>
#include <cstdlib>

#ifdef WIN32
#include <winsock2.h>
#endif

#include "./TestJsonCpp/TestJsonCpp.h"
#include "./TestMySQL/TestMySQL.h"
#include "./TestSerialization/TestSerialization.h"
#include "./TestNetService/TestNetService.h"
#include "./TestNetClient/TestNetClient.h"

#ifdef WIN32
CInitSock theSock;		// ≥ı ºªØWinsockø‚
#endif


int main(int argc, char **argv)
{
	std::cout << "Test Suite Main !" << std::endl;

	//UnitTestJsonCpp unit_test;
	//unit_test.Run();

	//TestMySQL unit_test;
	//unit_test.Run();

	//TestSerialization test_serialization;
	//test_serialization.Run();

	//TestNetService uint_test;
	//uint_test.Run();

	TestNetClient uint_test;
	uint_test.Run();

	std::cout << "Leave !" << std::endl;
	return 0;
}