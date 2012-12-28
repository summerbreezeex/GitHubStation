#include "UnitTest.h"

#include <iostream>

TestNet::TestNet(void)
{

}

TestNet::~TestNet(void)
{

}

int TestNet::Run(void)
{
	using namespace FREEZE_NET;
	Ctx context(2);

	std::cout << "Enter any key to eixt !" << std::endl;
	std::string end;
	std::cin >> end;

	return 0;
}
