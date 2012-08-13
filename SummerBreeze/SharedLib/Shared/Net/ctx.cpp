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

#include "platform.hpp"

#include <new>
#include <string.h>

#include "ctx.hpp"
#include "io_thread.hpp"
#include "err.hpp"
#include "config.hpp"
#include "zmq_listener.hpp"
#include "logical_thread.hpp"

#if !defined WIN32
#include <unistd.h>
#endif

zmq::ctx_t::ctx_t (uint32_t io_threads_) :
    tag (0xbadcafe0),
    terminating (false),
	main_threads (NULL),
	ptr_acceptor (NULL)
{
	//  Initialise the array of mailboxes. Additional three slots are for
	//  internal log socket and the zmq_term thread the reaper thread.
	slot_count = max_sockets + io_threads_ + 3;
	slots = (mailbox_t**) malloc (sizeof (mailbox_t*) * slot_count);
	alloc_assert (slots);

	//  Initialise the infrastructure for zmq_term thread.
	slots [term_tid] = &term_mailbox;

	//  Create the reaper thread.
	main_threads = new (std::nothrow) io_thread_t (this, main_threads_tid);
	alloc_assert (main_threads);
	slots [main_threads_tid] = main_threads->get_mailbox ();
	main_threads->start ();

	logical_threads = new (std::nothrow) logical_thread_t();
	alloc_assert (logical_threads);
	logical_threads->start();

	//  Create I/O thread objects and launch them.
	for (uint32_t i = 2; i != io_threads_ + 2; i++) {
		io_thread_t *io_thread = new (std::nothrow) io_thread_t (this, i);
		alloc_assert (io_thread);
		io_threads.push_back (io_thread);
		slots [i] = io_thread->get_mailbox ();
		io_thread->start ();
	}

	//  In the unused part of the slot array, create a list of empty slots.
	for (int32_t i = (int32_t) slot_count - 1;
		i >= (int32_t) io_threads_ + 2; i--) {
			empty_slots.push_back (i);
			slots [i] = NULL;
	}

	this->ptr_acceptor = new (std::nothrow) zmq_listener_t (this->main_threads);
	alloc_assert (ptr_acceptor);

	protocol_binary_t::add_prototype_instance();

}

bool zmq::ctx_t::check_tag ()
{
    return tag == 0xbadcafe0;
}

zmq::ctx_t::~ctx_t ()
{
}

int zmq::ctx_t::terminate ()
{
  return 0;
}


void zmq::ctx_t::send_command (uint32_t tid_, const command_t &command_)
{
    slots [tid_]->send (command_);
}

zmq::io_thread_t *zmq::ctx_t::choose_io_thread (uint64_t affinity_)
{
    if (io_threads.empty ())
        return NULL;

    //  Find the I/O thread with minimum load.
    int min_load = -1;
    io_threads_t::size_type result = 0;
    for (io_threads_t::size_type i = 0; i != io_threads.size (); i++) {
        if (!affinity_ || (affinity_ & (uint64_t (1) << i))) {
            int load = io_threads [i]->get_load ();
            if (min_load == -1 || load < min_load) {
                min_load = load;
                result = i;
            }
        }
    }
    zmq_assert (min_load != -1);
    return io_threads [result];
}



void zmq::ctx_t::log (const char *format_, va_list args_)
{
}


