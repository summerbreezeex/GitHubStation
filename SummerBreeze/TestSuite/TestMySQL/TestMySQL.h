#ifndef __TEST_MYSLQ_H__
#define __TEST_MYSLQ_H__

#include "../../SharedLib/Shared/MySQLdb/MySQLConnection.h"

class TestMySQL
{
public:
	TestMySQL(void);
	~TestMySQL(void);

public:
	int Run(void);

};


#endif