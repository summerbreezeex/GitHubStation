#include <iostream>
#include <cstdlib>


#include "./TestJsonCpp/TestJsonCpp.h"
#include "./TestMySQL/TestMySQL.h"

int main(int argc, char **argv)
{
	std::cout << "Test Suite Main !" << std::endl;

	//UnitTestJsonCpp unit_test;
	//unit_test.Run();

	TestMySQL unit_test;
	unit_test.Run();

	system("pause");
	return 0;
}