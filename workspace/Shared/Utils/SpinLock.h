#ifndef __SHARED_UTILS_SPINLOCK_H__
#define __SHARED_UTILS_SPINLOCK_H__

#include <pthread.h>

#include "Lock.h"

class SpinLock: public Lock
{
public:
	SpinLock(int count = SpinLock::TRY_COUNT);
	virtual ~SpinLock(void);

public:
	virtual void Acquire(void);
	virtual void Release(void);

protected:
	static const int TRY_COUNT;

protected:

	pthread_spinlock_t lock_;

};

#endif
