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

#ifndef __ZMQ_OBJECT_HPP_INCLUDED__
#define __ZMQ_OBJECT_HPP_INCLUDED__

#ifdef WIN32
#include <Winsock2.h>
#include <windows.h>
#include <process.h>
#include <assert.h>
#else
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#endif

#include "../Utils/Typedefs.h"

#include "select.hpp"
#include "ctx.hpp"
#include "command.hpp"

namespace zmq
{
    //  Base class for all objects that participate in inter-thread
    //  communication.

    class object_t
    {
    public:

        object_t (class ctx_t *ctx_, uint32_t tid_);
        object_t (object_t *parent_);
        virtual ~object_t ();

        uint32_t get_tid ();
        ctx_t *get_ctx ();
        void process_command (struct command_t &cmd_);

    protected:


        //  Logs an message.
        void log (const char *format_, ...);

        //  Chooses least loaded I/O thread.
        class io_thread_t *choose_io_thread (uint64_t affinity_);
		class io_thread_t *main_thread(void)
		{
			return ctx->main_thread();
		}
		void send_register_accept (class object_t *destination_, fd_t fd_, i_poll_events *events_);
		virtual void process_register_accept (struct command_t &cmd_);

		void send_new_connections (class object_t *destination_, fd_t fd_);
		virtual void process_new_connections (struct command_t &cmd_);

        //  Special handler called after a command that requires a seqnum
        //  was processed. The implementation should catch up with its counter
        //  of processed commands here.
        virtual void process_seqnum ();

    private:

        //  Context provides access to the global state.
        class ctx_t *ctx;

        //  Thread ID of the thread the object belongs to.
        uint32_t tid;

        void send_command (command_t &cmd_);

        object_t (const object_t&);
        const object_t &operator = (const object_t&);
    };

}

#endif
