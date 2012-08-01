#ifndef __SHARED_UTILS_ATOMIC_COUNTER_H__
#define __SHARED_UTILS_ATOMIC_COUNTER_H__

#include "Typedefs.h"
#include "SpinLock.h"
#include "Guard.h"


class AtomicCounterT
{
public:

	typedef uint32_t integer_t;

	inline AtomicCounterT (integer_t value = 0) :
	value_ (value)
	{
	}

	inline ~AtomicCounterT ()
	{
	}

	//  Set counter value (not thread-safe).
	inline void Set(integer_t value)
	{
		value_ = value;
	}

	//  Atomic addition. Returns the old value.
	inline integer_t Add(integer_t increment_)
	{
		integer_t old_value;

		Guard guard(this->sync_);

		old_value = this->value_;
		this->value_ += increment_;

		return old_value;
	}

	//  Atomic subtraction. Returns false if the counter drops to zero.
	inline bool Sub(integer_t decrement)
	{
		Guard guard(this->sync_);

		this->value_ -= decrement;
		bool result = this->value_ ? true : false;

		return result;
	}

	inline integer_t Get()
	{
		return this->value_;
	}

private:

	volatile integer_t value_;
	SpinLock sync_;

	AtomicCounterT (const AtomicCounterT&);
	const AtomicCounterT& operator = (const AtomicCounterT&);
};


#endif

