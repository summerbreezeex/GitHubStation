#ifndef __SHARED_UTILS_UTILS_H__
#define __SHARED_UTILS_UTILS_H__

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#undef DISALLOW_EVIL_CONSTRUCTORS
#define DISALLOW_EVIL_CONSTRUCTORS(TypeName)    \
	TypeName(const TypeName&);                  \
	void operator=(const TypeName&)


#include "Typedefs.h"

#include "AtomicCounter.hpp"
#include "CommandBase.h"
#include "Guard.h"
#include "Lock.h"
#include "Noncopyable.h"
#include "Singleton.h"
#include "SpinLock.h"


#endif