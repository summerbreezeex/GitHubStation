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

#include <string>

#include "tcp_connecter.hpp"
#include "platform.hpp"
#include "err.hpp"

#ifdef WIN32

zmq::tcp_connecter_t::tcp_connecter_t () :
    s (retired_fd)
{
    memset (&addr, 0, sizeof (addr));
    addr_len = 0;
}

zmq::tcp_connecter_t::~tcp_connecter_t ()
{
    //if (s != retired_fd)
    //    close ();
}

int zmq::tcp_connecter_t::set_address (sockaddr_in addr_)
{
	this->addr = addr_;
	this->addr_len = sizeof(this->addr);

	return 0;
}

int zmq::tcp_connecter_t::open ()
{
    zmq_assert (s == retired_fd);

    //  Create the socket.
    s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        wsa_error_to_errno ();
        return -1;
    }

    // Set to non-blocking mode.
    //unsigned long argp = 1;
    //int rc = ioctlsocket (s, FIONBIO, &argp);
    //wsa_assert (rc != SOCKET_ERROR);

    //  Disable Nagle's algorithm.
    int flag = 1;
    int rc = setsockopt (s, IPPROTO_TCP, TCP_NODELAY, (char*) &flag,
        sizeof (int));
    wsa_assert (rc != SOCKET_ERROR);

    //  Connect to the remote peer.
    rc = ::connect (s, (sockaddr*) &addr, addr_len);

    //  Connect was successfull immediately.
    if (rc == 0)
        return 0;

    //  Asynchronous connect was launched.
    if (rc == SOCKET_ERROR && (WSAGetLastError () == WSAEINPROGRESS ||
          WSAGetLastError () == WSAEWOULDBLOCK)) {
        errno = EAGAIN;
        return -1;
    }

    wsa_error_to_errno ();
    return -1;
}

int zmq::tcp_connecter_t::close ()
{
    zmq_assert (s != retired_fd);
    int rc = closesocket (s);
    wsa_assert (rc != SOCKET_ERROR);
    s = retired_fd;
    return 0;
}

zmq::fd_t zmq::tcp_connecter_t::get_fd ()
{
    return s;
}

zmq::fd_t zmq::tcp_connecter_t::connect ()
{
    //  Nonblocking connect have finished. Check whether an error occured.
    int err = 0;
    int len = sizeof err;
    int rc = getsockopt (s, SOL_SOCKET, SO_ERROR, (char*) &err, &len);
    zmq_assert (rc == 0);
    if (err != 0) {

        //  Assert that the error was caused by the networking problems
        //  rather than 0MQ bug.
        if (err == WSAECONNREFUSED || err == WSAETIMEDOUT ||
              err == WSAECONNABORTED || err == WSAEHOSTUNREACH ||
              err == WSAENETUNREACH || err == WSAENETDOWN)
            return retired_fd;

        wsa_assert_no (err);
    }

    //  Return the newly connected socket.
    fd_t result = s;
    s = retired_fd;
    return result;
}

#else

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>


zmq::tcp_connecter_t::tcp_connecter_t () :
    s (retired_fd)
{
    memset (&addr, 0, sizeof (addr));
}

zmq::tcp_connecter_t::~tcp_connecter_t ()
{
    //if (s != retired_fd)
    //    close ();
}

int zmq::tcp_connecter_t::set_address (sockaddr_in addr_)
{
	this->addr = addr_;
	this->addr_len = sizeof(this->addr);

    return 0;
}

int zmq::tcp_connecter_t::open ()
{
	zmq_assert (s == retired_fd);
	struct sockaddr *sa = (struct sockaddr*) &addr;

	//  Create the socket.
	s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == -1)
		return -1;


	int flags = fcntl (s, F_GETFL, 0);
	if (flags == -1)
		flags = 0;
	int rc = fcntl (s, F_SETFL, flags | O_NONBLOCK);
	errno_assert (rc != -1);


	//  Disable Nagle's algorithm.
	int flag = 1;
	rc = setsockopt (s, IPPROTO_TCP, TCP_NODELAY, (char*) &flag,
		sizeof (int));
	errno_assert (rc == 0);

	//  Connect to the remote peer.
	rc = ::connect (s, (struct sockaddr*) &addr, addr_len);

	//  Connect was successfull immediately.
	if (rc == 0)
		return 0;

	//  Asynchronous connect was launched.
	if (rc == -1 && errno == EINPROGRESS) {
		errno = EAGAIN;
		return -1;
	}

	//  Error occured.
	int err = errno;
	close ();
	errno = err;
	return -1;
}

int zmq::tcp_connecter_t::close ()
{
    zmq_assert (s != retired_fd);
    int rc = ::close (s);
    if (rc != 0)
        return -1;
    s = retired_fd;
    return 0;
}

zmq::fd_t zmq::tcp_connecter_t::get_fd ()
{
    return s;
}

zmq::fd_t zmq::tcp_connecter_t::connect ()
{
    //  Following code should handle both Berkeley-derived socket
    //  implementations and Solaris.
    int err = 0;
    int len = sizeof (err);
    int rc = getsockopt (s, SOL_SOCKET, SO_ERROR, (char*) &err, &len);
    if (rc == -1)
        err = errno;
    if (err != 0) {

        //  Assert if the error was caused by 0MQ bug.
        //  Networking problems are OK. No need to assert.
        errno = err;
        errno_assert (errno == ECONNREFUSED || errno == ECONNRESET ||
            errno == ETIMEDOUT || errno == EHOSTUNREACH ||
            errno == ENETUNREACH);

        return retired_fd;
    }

    fd_t result = s;
    s = retired_fd;
    return result;
}

#endif
