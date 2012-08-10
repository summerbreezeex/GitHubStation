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

#include <string.h>
#include <stdarg.h>

#include "object.hpp"
#include "ctx.hpp"
#include "err.hpp"
#include "io_thread.hpp"


zmq::object_t::object_t (ctx_t *ctx_, uint32_t tid_) :
    ctx (ctx_),
    tid (tid_)
{
}

zmq::object_t::object_t (object_t *parent_) :
    ctx (parent_->ctx),
    tid (parent_->tid)
{
}

zmq::object_t::~object_t ()
{
}

uint32_t zmq::object_t::get_tid ()
{
    return tid;
}

zmq::ctx_t *zmq::object_t::get_ctx ()
{
    return ctx;
}

void zmq::object_t::process_command (command_t &cmd_)
{
    switch (cmd_.type) 
	{
	case command_t::register_accept:
		process_register_accept(cmd_);
		break;

    default:
        zmq_assert (false);
    }

    //  The assumption here is that each command is processed once only,
    //  so deallocating it after processing is all right.
    deallocate_command (&cmd_);
}


void zmq::object_t::log (const char *format_, ...)
{
    va_list args;
    va_start (args, format_);
    ctx->log (format_, args);
    va_end (args);
}

zmq::io_thread_t *zmq::object_t::choose_io_thread (uint64_t affinity_)
{
    return ctx->choose_io_thread (affinity_);
}

void zmq::object_t::process_seqnum ()
{
    zmq_assert (false);
}

void zmq::object_t::send_command (command_t &cmd_)
{
    ctx->send_command (cmd_.destination->get_tid (), cmd_);
}

void zmq::object_t::send_register_accept (class object_t *destination_, fd_t fd_, i_poll_events *events_)
{
	command_t cmd;
	cmd.destination = destination_;
	cmd.type = command_t::register_accept;
	cmd.args.register_accept.fd = fd_;
	cmd.args.register_accept.events = events_;
	send_command (cmd);
}

void zmq::object_t::process_register_accept (struct command_t &cmd_)
{

}