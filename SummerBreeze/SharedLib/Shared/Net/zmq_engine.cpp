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
#include <new>

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

#include "zmq_engine.hpp"
#include "io_thread.hpp"
#include "err.hpp"
#include "concrete_protocol.hpp"
#include "logical_thread.hpp"

#include "../Serialization/PackPacket.h"
#include "../Serialization/UnpackPacket.h"

zmq::zmq_engine_t::zmq_engine_t (fd_t fd_) :
    plugged (false)
{
	this->io_thread = NULL;

	//  Initialise the underlying socket.
	int rc = tcp_socket.open (fd_, 0, 0);
	zmq_assert (rc == 0);
}

zmq::zmq_engine_t::~zmq_engine_t ()
{
    zmq_assert (!plugged);
}

void zmq::zmq_engine_t::plug (io_thread_t *io_thread_)
{
	zmq_assert (!plugged);
	plugged = true;
	this->io_thread = io_thread_;

	//  Connect to I/O threads poller object.
	io_object_t::plug (io_thread_);
	handle = add_fd (tcp_socket.get_fd ());
	set_pollin (handle);
	//set_pollout (handle);

	//  Flush all the data that may have been already received downstream.
	in_event ();

}

void zmq::zmq_engine_t::unplug ()
{
    zmq_assert (plugged);
    plugged = false;
	this->io_thread = NULL;

    //  Cancel all fd subscriptions.
    rm_fd (handle);

    //  Disconnect from I/O threads poller object.
    io_object_t::unplug ();

}

void zmq::zmq_engine_t::terminate ()
{
    unplug ();
    delete this;
}

void zmq::zmq_engine_t::in_event ()
{
	/* We have data on the fd waiting to be read. Read and
	display it. We must read whatever data is available
	completely, as we are running in edge-triggered mode
	and won't get a notification again for the same
	data. */

	while (1)
	{
		int count;
		char buf[1024];

		count = tcp_socket.read(buf, sizeof buf);
		if (count == -1)
		{
			this->error();
			return;
		}
		else if (count == 0)
		{
			break;
		}
		else
		{
			try 
			{
				pack_packet.Append(buf, count);

				uint32_t length = pack_packet.Length();
				if (length >= 16)
				{
					uint32_t packet_length = *((uint32_t*)(pack_packet.Buf() + 4));

					if (length >= packet_length)
					{
						char* ptr = pack_packet.Drain(packet_length);
						if (NULL == ptr)
						{
							this->error();
							return;
						}

						uint32_t opcode = *((uint32_t*)(ptr + 12));

						UnpackPacket unpack_packet(ptr, packet_length);

						protocol_binary_t* protocol_base = protocol_binary_t::find_and_clone(opcode);
						if (protocol_base != NULL)
						{
							unpack_packet >> protocol_base;

							if (NULL != this->io_thread)
							{
								this->io_thread->get_ctx()->logical_thread()->push(protocol_base);
							}
							else
							{
								delete protocol_base;
								protocol_base = NULL;

								this->error();
								return;
							}

						}
						else
						{
							delete ptr;
							ptr = NULL;

							this->error();
							return;
						}

						delete ptr;
						ptr = NULL;
					}

				}
			}
			catch( std::exception& e )
			{
				std::cout << e.what() << std::endl;
				this->error();
				return;
			}
			catch(...)
			{
				std::cout << "catch ..." << std::endl;
				this->error();
				return;
			}
		}
	}
}

void zmq::zmq_engine_t::out_event ()
{
}

void zmq::zmq_engine_t::error ()
{
	unplug ();
	delete this;
}