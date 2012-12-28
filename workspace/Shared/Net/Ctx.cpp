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

#include "Ctx.h"

#include <new>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "IOThread.h"
#include "MySQLRPCClient.h"
#include "MySQLRPCServer.h"

#define MAX_SOCKETS (512)

FREEZE_NET::Ctx::Ctx(uint32_t io_threads) :
	tag_(0xbadcafe0), terminating_(false)
{
	//  Initialise the array of mailboxes. Additional three slots are for
	//  internal log socket and the zmq_term thread the reaper thread.
	slot_count_ = MAX_SOCKETS + io_threads + enum_count;
	slots_ = (Mailbox**) malloc(sizeof(Mailbox*) * slot_count_);
	assert(slots_ != NULL);

	//  Initialise the infrastructure for zmq_term thread.
	slots_[term_tid] = &term_mailbox_;

	mysql_rpc_client_ = new (std::nothrow) MySQLRPCClient(this,
			rpc_mysql_client_tid);
	assert(mysql_rpc_client_ != NULL);
	slots_[rpc_mysql_client_tid] = mysql_rpc_client_->GetMailbox();
	mysql_rpc_client_->Start();

	mysql_rpc_server_ = new (std::nothrow) MySQLRPCServer(this,
			rpc_mysql_server_tid);
	assert(mysql_rpc_server_ != NULL);
	slots_[rpc_mysql_server_tid] = mysql_rpc_server_->GetMailbox();
	mysql_rpc_server_->Start();


	//  Create I/O thread objects and launch them.
	for (uint32_t i = enum_count; i != io_threads + enum_count; i++)
	{
		IOThread *io_thread = new (std::nothrow) IOThread(this, i);
		assert(io_thread != NULL);
		io_threads_.push_back(io_thread);
		slots_[i] = io_thread->GetMailbox();
		io_thread->Start();
	}

	//  In the unused part of the slot array, create a list of empty slots.
	for (int32_t i = (int32_t) slot_count_ - 1; i >= (int32_t) io_threads
			+ enum_count; i--)
	{
		empty_slots_.push_back(i);
		slots_[i] = NULL;
	}
}

bool FREEZE_NET::Ctx::CheckTag()
{
	return tag_ == 0xbadcafe0;
}

FREEZE_NET::Ctx::~Ctx()
{
	//  Remove the tag, so that the object is considered dead.
	tag_ = 0xdeadbeef;
}

int FREEZE_NET::Ctx::Terminate()
{
	return 0;
}

void FREEZE_NET::Ctx::SendCommand(uint32_t tid, const Command& command)
{
	slots_[tid]->Send(command);
}

FREEZE_NET::IOThread *FREEZE_NET::Ctx::ChooseIOThread(uint64_t affinity)
{
	if (io_threads_.empty())
	{
		return NULL;
	}

	//  Find the I/O thread with minimum load.
	int min_load = -1;
	io_threads_t::size_type result = 0;
	for (io_threads_t::size_type i = 0; i != io_threads_.size(); i++)
	{
		int load = io_threads_[i]->GetLoad();
		if (min_load == -1 || load < min_load)
		{
			min_load = load;
			result = i;
		}
	}
	assert(min_load != -1);
	return io_threads_[result];
}

FREEZE_NET::MySQLRPCClient* FREEZE_NET::Ctx::MysqlRPCClient()
{
	return mysql_rpc_client_;
}

FREEZE_NET::MySQLRPCServer* FREEZE_NET::Ctx::MysqlRPCServer()
{
	return mysql_rpc_server_;
}

