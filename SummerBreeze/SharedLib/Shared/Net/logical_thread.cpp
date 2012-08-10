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

#include <new>

#include "logical_thread.hpp"
#include "platform.hpp"
#include "err.hpp"
#include "ctx.hpp"
#include "zmq_engine.hpp"
#include "clock.hpp"

zmq::logical_thread_t::logical_thread_t () :
	stopping (false)
{
	producer = new (std::nothrow) std::queue<protocol_binary_t *>;
	alloc_assert (producer);

	consumer = new (std::nothrow) std::queue<protocol_binary_t *>;
	alloc_assert (consumer);
}

zmq::logical_thread_t::~logical_thread_t ()
{
}

void zmq::logical_thread_t::start ()
{
	worker.start (worker_routine, this);
}

void zmq::logical_thread_t::stop ()
{
	stopping = true;
	worker.stop ();
}

void zmq::logical_thread_t::worker_routine (void *arg_)
{
	 ((logical_thread_t*) arg_)->loop ();
}


void zmq::logical_thread_t::loop ()
{
	while (!stopping) {

		//  Execute any due timers.
		int timeout = (int) execute_timers ();

		while(!this->consumer->empty())
		{
			protocol_binary_t* temp = this->front();
			this->pop();

			if (temp != NULL)
			{
				temp->run();
				delete temp;
				temp = NULL;
			}
			
		}

		this->swap();

		zmq_sleep(0);
	}
}


void zmq::logical_thread_t::swap(void)
{
	Guard guard(this->lock);

	if (!this->consumer->empty())
	{
		return;
	}
	
	std::queue<protocol_binary_t *> *temp = this->consumer;
	this->consumer = this->producer;
	this->producer = temp;
}

void zmq::logical_thread_t::push (protocol_binary_t* ptr_value)
{
	Guard guard(this->lock);

	this->producer->push(ptr_value);
}


void zmq::logical_thread_t::pop (void)
{
	this->consumer->pop();
}

protocol_binary_t* zmq::logical_thread_t::front (void)
{
	return this->consumer->front();
}

bool zmq::logical_thread_t::empty (void) const
{
	return this->consumer->empty();
}


