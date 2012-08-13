/*
    Copyright (c) 2007-2011 iMatix Corporation
    Copyright (c) 2007-2011 Other contributors as noted in the AUTHORS file

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "clock.hpp"
#include "likely.hpp"
#include "err.hpp"

#include <stddef.h>

#if defined _MSC_VER
#include <intrin.h>
#endif

#if !defined WIN32
#include <sys/time.h>
#endif

#include <time.h>


zmq::clock_t::clock_t () :
    last_tsc (0),
    last_time (0)
{
}

zmq::clock_t::~clock_t ()
{
}

uint64_t zmq::clock_t::now_us ()
{
	return 0;
}

uint64_t zmq::clock_t::now_ms ()
{
	return time(NULL) * 1000;
}

uint64_t zmq::clock_t::rdtsc ()
{
    return 0;
}

void zmq::zmq_sleep (int microsecond_)
{
#if defined WIN32
	Sleep (microsecond_ / 1000);
#else
	usleep (microsecond_);
#endif
}