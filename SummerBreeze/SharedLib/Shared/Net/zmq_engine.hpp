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

#ifndef __ZMQ_ZMQ_ENGINE_HPP_INCLUDED__
#define __ZMQ_ZMQ_ENGINE_HPP_INCLUDED__

#include <stddef.h>

#include <string>

#include "../Serialization/PackPacket.h"
#include "../Serialization/UnpackPacket.h"

#include "io_object.hpp"
#include "tcp_socket.hpp"


namespace zmq
{

    class zmq_engine_t : public io_object_t
    {
    public:

        zmq_engine_t (fd_t fd_);
        ~zmq_engine_t ();

        //  i_engine interface implementation.
        void plug (class io_thread_t *io_thread_);
        void unplug ();
        void terminate ();

        //  i_poll_events interface implementation.
        void in_event ();
        void out_event ();

		io_thread_t *get_io_thread ()
		{
			return this->io_thread;
		}
    private:

		io_thread_t *io_thread;

        //  Function to handle network disconnections.
        void error ();

        tcp_socket_t tcp_socket;
        handle_t handle;

        bool plugged;

		PackPacket pack_packet;

        zmq_engine_t (const zmq_engine_t&);
        const zmq_engine_t &operator = (const zmq_engine_t&);
    };

}

#endif
