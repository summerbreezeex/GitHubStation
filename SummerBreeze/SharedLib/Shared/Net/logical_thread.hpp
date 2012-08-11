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

#ifndef __ZMQ_LOGICAL_THREAD_HPP_INCLUDED__
#define __ZMQ_LOGICAL_THREAD_HPP_INCLUDED__

#include <vector>

#include "object.hpp"
#include "poller.hpp"
#include "i_poll_events.hpp"
#include "mailbox.hpp"
#include "protocol_binary.hpp"

#include "../Utils/Guard.h"
#include "../Utils/SpinLock.h"

namespace zmq
{


    class logical_thread_t : public poller_base_t
    {
    public:

        logical_thread_t ();
        ~logical_thread_t ();

        //  Launch the physical thread.
        void start ();

        //  Ask underlying thread to stop.
        void stop ();

		//生产者往producer 推送数据包
		void push (protocol_binary_t* ptr_value);

	private:
		void pop (void);
		protocol_binary_t* front (void);
		bool empty (void) const;
		void swap(void);

	private:

		//  Main worker thread routine.
		static void worker_routine (void *arg_);

		//  Main event loop.
		void loop ();

		//  If true, thread is shutting down.
		bool stopping;

		thread_t worker;

		std::queue<protocol_binary_t *>* producer;
		std::queue<protocol_binary_t *>* consumer;

		SpinLock lock;

		logical_thread_t (const logical_thread_t&);
		const logical_thread_t &operator = (const logical_thread_t&);
	};

}

#endif
