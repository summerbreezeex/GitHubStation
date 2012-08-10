#include <iostream>
#include <cstdlib>

#include <winsock2.h>

#include "./TestJsonCpp/TestJsonCpp.h"
#include "./TestMySQL/TestMySQL.h"
#include "./TestSerialization/TestSerialization.h"
#include "./TestNetService/TestNetService.h"

CInitSock theSock;		// ≥ı ºªØWinsockø‚

int main(int argc, char **argv)
{
	std::cout << "Test Suite Main !" << std::endl;

	//UnitTestJsonCpp unit_test;
	//unit_test.Run();

	//TestMySQL unit_test;
	//unit_test.Run();

	//TestSerialization test_serialization;
	//test_serialization.Run();

	TestNetService uint_test;
	uint_test.Run();

	std::cout << "Leave !" << std::endl;
	return 0;
}