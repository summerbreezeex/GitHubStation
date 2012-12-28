#include "SpinLock.h"

const int SpinLock::TRY_COUNT = 5;

SpinLock::SpinLock(int count)
{
	::pthread_spin_init(&lock_, count);
}

SpinLock::~SpinLock(void)
{
	::pthread_spin_destroy(&lock_);
}

void SpinLock::Acquire(void)
{
	::pthread_spin_lock(&lock_);
}

void SpinLock::Release(void)
{
	::pthread_spin_unlock(&lock_);
}
