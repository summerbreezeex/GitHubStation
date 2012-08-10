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



//  On AIX, poll.h has to be included before zmq.h to get consistent
//  definition of pollfd structure (AIX uses 'reqevents' and 'retnevents'
//  instead of 'events' and 'revents' and defines macros to map from POSIX-y
//  names to AIX-specific names).
#if defined WIN32
#include <Winsock2.h>
#include <windows.h>
#else
#include <sys/select.h>
#endif


#include "signaler.hpp"
#include "likely.hpp"
#include "err.hpp"
#include "fd.hpp"


#if defined WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif

zmq::signaler_t::signaler_t ()
{
    //  Create the socketpair for signaling.
    int rc = make_fdpair (&r, &w);
    errno_assert (rc == 0);

    //  Set both fds to non-blocking mode.
#if defined WIN32
    unsigned long argp = 1;
    rc = ioctlsocket (w, FIONBIO, &argp);
    wsa_assert (rc != SOCKET_ERROR);
    rc = ioctlsocket (r, FIONBIO, &argp);
    wsa_assert (rc != SOCKET_ERROR);
#else
    int flags = fcntl (w, F_GETFL, 0);
    errno_assert (flags >= 0);
    rc = fcntl (w, F_SETFL, flags | O_NONBLOCK);
    errno_assert (rc == 0);
    flags = fcntl (r, F_GETFL, 0);
    errno_assert (flags >= 0);
    rc = fcntl (r, F_SETFL, flags | O_NONBLOCK);
    errno_assert (rc == 0);
#endif
}

zmq::signaler_t::~signaler_t ()
{
#if defined WIN32
    int rc = closesocket (w);
    wsa_assert (rc != SOCKET_ERROR);
    rc = closesocket (r);
    wsa_assert (rc != SOCKET_ERROR);
#else
    close (w);
    close (r);
#endif
}

zmq::fd_t zmq::signaler_t::get_fd ()
{
    return r;
}

void zmq::signaler_t::send ()
{
#if defined WIN32
    unsigned char dummy = 0;
    int nbytes = ::send (w, (char*) &dummy, sizeof (dummy), 0);
    wsa_assert (nbytes != SOCKET_ERROR);
    zmq_assert (nbytes == sizeof (dummy));
#else
    unsigned char dummy = 0;
    while (true) {
        ssize_t nbytes = ::send (w, &dummy, sizeof (dummy), 0);
        if (unlikely (nbytes == -1 && errno == EINTR))
            continue;
        zmq_assert (nbytes == sizeof (dummy));
        break;
    }
#endif
}

int zmq::signaler_t::wait (int timeout_)
{
	fd_set fds;
	FD_ZERO (&fds);
	FD_SET (r, &fds);
	struct timeval timeout;
	if (timeout_ >= 0) {
		timeout.tv_sec = timeout_ / 1000;
		timeout.tv_usec = timeout_ % 1000 * 1000;
	}

#ifdef WIN32
    int rc = select (0, &fds, NULL, NULL,
        timeout_ >= 0 ? &timeout : NULL);
    wsa_assert (rc != SOCKET_ERROR);
#else
    int rc = select (r + 1, &fds, NULL, NULL,
        timeout_ >= 0 ? &timeout : NULL);
    if (unlikely (rc < 0)) {
        zmq_assert (errno == EINTR);
        return -1;
    }
#endif
    if (unlikely (rc == 0)) {
        errno = EAGAIN;
        return -1;
    }
    zmq_assert (rc == 1);
    return 0;
}

int zmq::signaler_t::recv ()
{
    //  Attempt to read a signal.
    unsigned char dummy;
#ifdef WIN32
    int nbytes = ::recv (r, (char*) &dummy, sizeof (dummy), 0);
    wsa_assert (nbytes != SOCKET_ERROR);
#else
    ssize_t nbytes = ::recv (r, &dummy, sizeof (dummy), 0);
    errno_assert (nbytes >= 0);
#endif
    //zmq_assert (nbytes == sizeof (dummy));
    //zmq_assert (dummy == 0);

	return nbytes;
}

int zmq::signaler_t::make_fdpair (fd_t *r_, fd_t *w_)
{
#if defined WIN32

    //  Windows has no 'socketpair' function. CreatePipe is no good as pipe
    //  handles cannot be polled on. Here we create the socketpair by hand.
    *w_ = INVALID_SOCKET;
    *r_ = INVALID_SOCKET;

    //  Create listening socket.
    SOCKET listener;
    listener = socket (AF_INET, SOCK_STREAM, 0);
    wsa_assert (listener != INVALID_SOCKET);


    //  Set SO_REUSEADDR and TCP_NODELAY on listening socket.
    BOOL so_reuseaddr = 1;
    int rc = setsockopt (listener, SOL_SOCKET, SO_REUSEADDR,
        (char *)&so_reuseaddr, sizeof (so_reuseaddr));
    wsa_assert (rc != SOCKET_ERROR);
    BOOL tcp_nodelay = 1;
    rc = setsockopt (listener, IPPROTO_TCP, TCP_NODELAY,
        (char *)&tcp_nodelay, sizeof (tcp_nodelay));
    wsa_assert (rc != SOCKET_ERROR);

    //  Bind listening socket to any free local port.
    struct sockaddr_in addr;
    memset (&addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);
    addr.sin_port = 0;
    rc = bind (listener, (const struct sockaddr*) &addr, sizeof (addr));
    wsa_assert (rc != SOCKET_ERROR);

    //  Retrieve local port listener is bound to (into addr).
    int addrlen = sizeof (addr);
    rc = getsockname (listener, (struct sockaddr*) &addr, &addrlen);
    wsa_assert (rc != SOCKET_ERROR);

    //  Listen for incomming connections.
    rc = listen (listener, 1);
    wsa_assert (rc != SOCKET_ERROR);

    //  Create the writer socket.
    *w_ = WSASocket (AF_INET, SOCK_STREAM, 0, NULL, 0,  0);
    wsa_assert (*w_ != INVALID_SOCKET);

    ////  Set TCP_NODELAY on writer socket.
	rc = setsockopt (*w_, IPPROTO_TCP, TCP_NODELAY,
		(char *)&tcp_nodelay, sizeof (tcp_nodelay));
	wsa_assert (rc != SOCKET_ERROR);

    //  Connect writer to the listener.
    rc = connect (*w_, (sockaddr *) &addr, sizeof (addr));
    wsa_assert (rc != SOCKET_ERROR);

    //  Accept connection from writer.
    *r_ = accept (listener, NULL, NULL);
    wsa_assert (*r_ != INVALID_SOCKET);

    //  We don't need the listening socket anymore. Close it.
    rc = closesocket (listener);
    wsa_assert (rc != SOCKET_ERROR);

    return 0;

#else // All other implementations support socketpair()

    int sv [2];
    int rc = socketpair (AF_UNIX, SOCK_STREAM, 0, sv);
    errno_assert (rc == 0);
    *w_ = sv [0];
    *r_ = sv [1];
    return 0;

#endif
}

