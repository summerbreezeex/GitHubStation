#include <iostream>
#include <cstdlib>
#include <string>

#include "TestCase/TestCase.h"
#include "MySQLDb/UnitTest.h"
#include "Net/UnitTest.h"
#include "RPCMySQL/UnitTest.h"

#include "ByteBuffer.h"

int main (int argc, char *argv [])
{
	std::cout << "hello world !" << std::endl;


	TestRPCMysql unit_test;
	unit_test.Run();


	std::cout << "enter any key to end!" << std::endl;
	std::string end;
	std::cin >> end;

	return 0;
}
