#ifndef __UNITTEST_TESTCASE_TESTCASE_H_H__
#define __UNITTEST_TESTCASE_TESTCASE_H_H__


class TestCase
{
public:
	TestCase();
	virtual ~TestCase();

public:
	virtual void Run(void);
};

#endif
