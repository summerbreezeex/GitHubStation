#ifndef __TEST_NET_CONTEXT_H__
#define __TEST_NET_CONTEXT_H__

#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// Á´½Óµ½WS2_32.lib
#endif


#include "../../SharedLib/Shared/Net/ctx.hpp"
#include "../../SharedLib/Shared/Net/initsock.h"

class TestNetContext
{
public:
	TestNetContext(void);
	~TestNetContext(void);

public:
	int Run(void);

};


#endif