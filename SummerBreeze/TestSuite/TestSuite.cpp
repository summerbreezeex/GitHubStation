#include <iostream>
#include <cstdlib>


#include "./TestJsonCpp/TestJsonCpp.h"
#include "./TestMySQL/TestMySQL.h"
#include "./TestSerialization/TestSerialization.h"

int main(int argc, char **argv)
{
	std::cout << "Test Suite Main !" << std::endl;

	//UnitTestJsonCpp unit_test;
	//unit_test.Run();

	//TestMySQL unit_test;
	//unit_test.Run();

	TestSerialization test_serialization;
	test_serialization.Run();

	system("pause");
	return 0;
}