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

#include "zmq_listener.hpp"
#include "io_thread.hpp"
#include "err.hpp"

zmq::zmq_listener_t::zmq_listener_t (io_thread_t *io_thread_) :
    object_t (io_thread_),
    io_object_t (io_thread_)
{
}

zmq::zmq_listener_t::~zmq_listener_t ()
{
}

int zmq::zmq_listener_t::set_address (sockaddr_in addr_, int backlog_)
{
     return tcp_listener.set_address (addr_, backlog_);
}

void zmq::zmq_listener_t::process_plug ()
{
	//  Start polling for incoming connections.
	//handle = add_fd (tcp_listener.get_fd ());
	//set_pollin (handle);

	io_thread_t *main_thread_info = main_thread();
	this->send_register_accept(main_thread_info, tcp_listener.get_fd (), this);
}

void zmq::zmq_listener_t::process_term (int linger_)
{
    rm_fd (handle);
}

void zmq::zmq_listener_t::in_event ()
{
    fd_t fd = tcp_listener.accept ();

    //  If connection was reset by the peer in the meantime, just ignore it.
    //  TODO: Handle specific errors like ENFILE/EMFILE etc.
    if (fd == retired_fd)
        return;

#if defined WIN32
	unsigned long argp = 1;
	int rc = ioctlsocket (fd, FIONBIO, &argp);
	wsa_assert (rc != SOCKET_ERROR);

#else
	int flags = fcntl (w, F_GETFL, 0);
	errno_assert (flags >= 0);
	int rc = fcntl (w, F_SETFL, flags | O_NONBLOCK);
	errno_assert (rc == 0);
#endif

    //  Choose I/O thread to run connecter in. Given that we are already
    //  running in an I/O thread, there must be at least one available.
    io_thread_t *io_thread = choose_io_thread (0);
    zmq_assert (io_thread);

    //  Create and launch an init object. 

}

