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

#include "mailbox.hpp"
#include "platform.hpp"
#include "err.hpp"

zmq::mailbox_t::mailbox_t ()
{
    active = false;
}

zmq::mailbox_t::~mailbox_t ()
{
    //  TODO: Retrieve and deallocate commands inside the cpipe.

    // Work around problem that other threads might still be in our
    // send() method, by waiting on the mutex before disappearing.
    sync.lock ();
    sync.unlock ();
}

zmq::fd_t zmq::mailbox_t::get_fd ()
{
    return signaler.get_fd ();
}

void zmq::mailbox_t::send (const command_t &cmd_)
{
	sync.lock ();

	cpipe.push(cmd_);
	signaler.send ();

	sync.unlock ();

}

int zmq::mailbox_t::recv (command_t *cmd_, int timeout_)
{

	int rc = signaler.recv ();
	if (sizeof (char) != rc)
	{
		errno = EAGAIN;
		return rc;
	}
	

	if (cpipe.size() > 0)
	{
		*cmd_ = cpipe.front();
		cpipe.pop();

		return 0;
	}

	errno = EAGAIN;
	return 1;
}

