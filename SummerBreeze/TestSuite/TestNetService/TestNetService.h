#ifndef __TEST_NET_SERVICE_H__
#define __TEST_NET_SERVICE_H__

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// Á´½Óµ½WS2_32.lib

#include "../../SharedLib/Shared/Net/ctx.hpp"
#include "../../SharedLib/Shared/Net/initsock.h"

class TestNetService
{
public:
	TestNetService(void);
	~TestNetService(void);

public:
	int Run(void);

};


#endif