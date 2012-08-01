#ifndef __SHARED_UTILS_UTILS_H__
#define __SHARED_UTILS_UTILS_H__

#ifdef WIN32
#include <windows.h>
#endif

#undef DISALLOW_EVIL_CONSTRUCTORS
#define DISALLOW_EVIL_CONSTRUCTORS(TypeName)    \
	TypeName(const TypeName&);                  \
	void operator=(const TypeName&)


#include "Typedefs.h"
#include "Noncopyable.h"

#endif