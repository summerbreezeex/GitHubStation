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

#ifndef __ZMQ_COMMAND_HPP_INCLUDED__
#define __ZMQ_COMMAND_HPP_INCLUDED__

#include "../Utils/Typedefs.h"

#include "fd.hpp"

namespace zmq
{

    //  This structure defines the commands that can be sent between threads.

    struct command_t
    {
		command_t(void)
		{
			this->destination = NULL;
			this->type = invalid;
		}

        //  Object to process the command.
        class object_t *destination;

        enum type_t
        {
			invalid          = 0,//无效的
			register_accept  = 1,//
			new_connections  = 2,//新连接
            stop,
            plug,
            own,
            attach,
            bind,
            activate_reader,
            activate_writer,
            pipe_term,
            pipe_term_ack,
            term_req,
            term,
            term_ack,
            reap,
            reaped,
            done
        } type;

        union {

			struct {
			} invalid;

			struct {
				fd_t fd;
				class i_poll_events *events;
			} register_accept;

            //  Sent to I/O thread to let it know that it should
            //  terminate itself.
            struct {
            } stop;

            //  Sent to I/O object to make it register with its I/O thread.
            struct {
            } plug;

        } args;
    };

    //  Function to deallocate dynamically allocated components of the command.
    void deallocate_command (command_t *cmd_);

}    

#endif
