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

#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <errno.h>

#include "Signaler.h"

FREEZE_NET::Signaler::Signaler()
{
    //  Create the socketpair for signaling.
    int rc = make_fdpair(&r_, &w_);
    assert(rc == 0);

    int flags = fcntl(w_, F_GETFL, 0);
    assert(flags >= 0);
    rc = fcntl(w_, F_SETFL, flags | O_NONBLOCK);
    assert(rc == 0);
    flags = fcntl(r_, F_GETFL, 0);
    assert(flags >= 0);
    rc = fcntl(r_, F_SETFL, flags | O_NONBLOCK);
    assert(rc == 0);
}

FREEZE_NET::Signaler::~Signaler()
{
    close(w_);
    close(r_);
}

FREEZE_NET::fd_t FREEZE_NET::Signaler::GetFd()
{
    return r_;
}

void FREEZE_NET::Signaler::Send()
{
    unsigned char dummy = 0;
    while (true)
    {
        ssize_t nbytes = ::send(w_, &dummy, sizeof(dummy), 0);
        if (nbytes == -1 && (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
        {
        	 continue;
        }
        assert(nbytes == sizeof(dummy));
        break;
    }
}

void FREEZE_NET::Signaler::Recv()
{
    //  Attempt to read a signal.
    unsigned char dummy;
    ssize_t nbytes = ::recv(r_, &dummy, sizeof(dummy), 0);
    if (nbytes == -1 && (errno == EAGAIN || errno == EINTR))
    {
    	return;
    }

    assert(nbytes == sizeof(dummy));
    assert(dummy == 0);
}

int FREEZE_NET::Signaler::make_fdpair(fd_t *r, fd_t *w)
{
    int sv [2];
    int rc = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
    assert(rc == 0);
    *w = sv [0];
    *r = sv [1];
    return 0;
}


