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

#include "io_thread.hpp"
#include "platform.hpp"
#include "err.hpp"
#include "ctx.hpp"
#include "zmq_engine.hpp"

zmq::io_thread_t::io_thread_t (ctx_t *ctx_, uint32_t tid_) :
    object_t (ctx_, tid_)
{
    poller = new (std::nothrow) poller_t;
    alloc_assert (poller);

    mailbox_handle = poller->add_fd (mailbox.get_fd (), this);
    poller->set_pollin (mailbox_handle);
}

zmq::io_thread_t::~io_thread_t ()
{
    delete poller;
}

void zmq::io_thread_t::start ()
{
    //  Start the underlying I/O thread.
    poller->start ();
}

void zmq::io_thread_t::stop ()
{
	send_stop ();
}

zmq::mailbox_t *zmq::io_thread_t::get_mailbox ()
{
    return &mailbox;
}

int zmq::io_thread_t::get_load ()
{
    return poller->get_load ();
}

void zmq::io_thread_t::in_event ()
{
    //  TODO: Do we want to limit number of commands I/O thread can
    //  process in a single go?

    while (true) {

        //  Get the next command. If there is none, exit.
        command_t cmd;
        int rc = mailbox.recv (&cmd, 0);
        if (rc != 0 && errno == EINTR)
            continue;
        if (rc != 0 && errno == EAGAIN)
            break;
        errno_assert (rc == 0);

        //  Process the command.
        cmd.destination->process_command (cmd);
    }
}

void zmq::io_thread_t::out_event ()
{
    //  We are never polling for POLLOUT here. This function is never called.
    zmq_assert (false);
}

void zmq::io_thread_t::timer_event (int id_)
{
    //  No timers here. This function is never called.
    zmq_assert (false);
}

zmq::poller_t *zmq::io_thread_t::get_poller ()
{
    zmq_assert (poller);
    return poller;
}

void zmq::io_thread_t::process_stop ()
{
	std::vector<zmq_engine_t*> delete_items;

	for (engine_map_t::iterator ite = this->engine_map.begin();
		ite != this->engine_map.end();
		ite++)
	{
		delete_items.push_back(ite->second);
	}
	
	for (std::vector<zmq_engine_t*>::iterator del_ite = delete_items.begin();
		del_ite != delete_items.end();
		del_ite++)
	{
		(*del_ite)->terminate();
	}
	
    poller->rm_fd (mailbox_handle);
    poller->stop ();
}

void zmq::io_thread_t::process_register_accept (struct command_t &cmd_)
{
	poller_t::handle_t  handle = poller->add_fd (cmd_.args.register_accept.fd, cmd_.args.register_accept.events);
	poller->set_pollin (handle);
}

void zmq::io_thread_t::process_new_connections (struct command_t &cmd_)
{
	fd_t fd = cmd_.args.new_connections.fd;

	uint32_t serial = serial_num.add(1);

	zmq_engine_t * ptr_engine = new (std::nothrow) zmq_engine_t(fd, serial);
	alloc_assert (ptr_engine);

	engine_map[serial] = ptr_engine;

	ptr_engine->plug(this);
}