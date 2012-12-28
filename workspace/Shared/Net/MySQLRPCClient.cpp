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

#include <new>

#include "MySQLRPCClient.h"
#include "Ctx.h"
#include "Command.h"
#include "MySQLRPCServer.h"

#include "../Utils/CommandBase.h"

FREEZE_NET::MySQLRPCClient::MySQLRPCClient(Ctx* ctx, uint32_t tid) :
	Object(ctx, tid)
{
	poller_ = new (std::nothrow) Poller;
	assert(poller_ != NULL);

	mailbox_handle_ = poller_->AddFd(mailbox_.GetFd(), this);
	poller_->SetPollin(mailbox_handle_);
}

FREEZE_NET::MySQLRPCClient::~MySQLRPCClient()
{
	delete poller_;
}

void FREEZE_NET::MySQLRPCClient::Start()
{
	//  Start the underlying I/O thread.
	poller_->Start();
}

void FREEZE_NET::MySQLRPCClient::Stop()
{
	SendStop();
}

FREEZE_NET::Mailbox *FREEZE_NET::MySQLRPCClient::GetMailbox()
{
	return &mailbox_;
}

int FREEZE_NET::MySQLRPCClient::GetLoad()
{
	return poller_->GetLoad();
}

void FREEZE_NET::MySQLRPCClient::InEvent()
{
	//  TODO: Do we want to limit number of commands I/O thread can
	//  process in a single go?

	while (true)
	{
		//  Get the next command. If there is none, exit.
		Command cmd;
		int rc = mailbox_.Recv(&cmd);
		if (rc != 0 && errno == EINTR)
			continue;
		if (rc != 0 && errno == EAGAIN)
			break;
		assert (rc == 0);

		//  Process the command.
		cmd.destination_->ProcessCommand(cmd);
	}
}

void FREEZE_NET::MySQLRPCClient::OutEvent()
{
	//  We are never polling for POLLOUT here. This function is never called.
	assert(false);
}

void FREEZE_NET::MySQLRPCClient::TimerEvent(int id)
{
	//  No timers here. This function is never called.
	assert(false);
}

FREEZE_NET::Poller *FREEZE_NET::MySQLRPCClient::GetPoller()
{
	assert(poller_ != NULL);
	return poller_;
}

void FREEZE_NET::MySQLRPCClient::ProcessStop()
{
	poller_->RmFd(mailbox_handle_);
	poller_->Stop();
}

void FREEZE_NET::MySQLRPCClient::SendRPC(MySQLRPCServer* destination,
		char const* table, char const* query_statement,
		CommandBaseEx* callback_cmd,
		ResultSet* query_result)
{
	uint32_t msg_id = msg_id_.Add(1);

	Command cmd;
	cmd.destination_ = destination;
	cmd.type_ = Command::RPC_MYSQL_REQUEST;

	cmd.args_.RPC_MYSQL_REQUEST.msg_id_ = msg_id;

	std::size_t ptr_table_size = strlen(table) + 1;
	char* ptr_table = new char[ptr_table_size];
	memset(ptr_table, 0, ptr_table_size);
	memcpy(ptr_table, table, ptr_table_size);

	std::size_t ptr_query_statement_size = strlen(query_statement) + 1;
	char* ptr_query_statement = new char[ptr_query_statement_size];
	memset(ptr_query_statement, 0, ptr_query_statement_size);
	memcpy(ptr_query_statement, query_statement, ptr_query_statement_size);

	cmd.args_.RPC_MYSQL_REQUEST.table_ = ptr_table;
	cmd.args_.RPC_MYSQL_REQUEST.query_statement_ = ptr_query_statement;
	cmd.args_.RPC_MYSQL_REQUEST.callback_cmd_ = callback_cmd;
	cmd.args_.RPC_MYSQL_REQUEST.query_result_ = query_result;
	cmd.args_.RPC_MYSQL_REQUEST.request_object_ = this;

	SendRPCMysqlRequest(destination, cmd.args_.RPC_MYSQL_REQUEST.msg_id_,
			cmd.args_.RPC_MYSQL_REQUEST.table_,
			cmd.args_.RPC_MYSQL_REQUEST.query_statement_,
			cmd.args_.RPC_MYSQL_REQUEST.callback_cmd_,
			cmd.args_.RPC_MYSQL_REQUEST.query_result_,
			cmd.args_.RPC_MYSQL_REQUEST.request_object_);

	client_stubs_[msg_id] = cmd;
}

void FREEZE_NET::MySQLRPCClient::ProcessRPCMysqlResponse(uint32_t msg_id,
		uint8_t return_result, ResultSet* query_result)
{
	std::map<uint32_t, Command>::iterator ite = client_stubs_.find(msg_id);
	if (ite == client_stubs_.end())
	{
		std::cout << "mysql rpc response mapped error !" << std::endl;
	}
	else
	{
		if (return_result == 0)
		{
			std::cout << "mysql rpc query statement execute error !"
					<< std::endl;
		}

		switch (ite->second.type_)
		{
		case Command::RPC_MYSQL_REQUEST:
		{
			if (ite->second.args_.RPC_MYSQL_REQUEST.callback_cmd_ != NULL )
			{
				ite->second.args_.RPC_MYSQL_REQUEST.callback_cmd_->Run(query_result);
			}
			else
			{
				std::cout << "ite->second.args_.RPC_MYSQL_REQUEST.callback_cmd_ == NULL" << std::endl;
			}
			break;
		}
		default:
		{
			std::cout << "mysql rpc request tyep error !" << std::endl;
			break;
		}
		}

		if (ite->second.args_.RPC_MYSQL_REQUEST.table_ != NULL)
		{
			delete [] ite->second.args_.RPC_MYSQL_REQUEST.table_;
			ite->second.args_.RPC_MYSQL_REQUEST.table_ = NULL;
		}
		if (ite->second.args_.RPC_MYSQL_REQUEST.query_statement_ != NULL)
		{
			delete [] ite->second.args_.RPC_MYSQL_REQUEST.query_statement_;
			ite->second.args_.RPC_MYSQL_REQUEST.query_statement_ = NULL;
		}

		client_stubs_.erase(ite);
	}
}


void FREEZE_NET::MySQLRPCClient::HandlePRCResponse(ResultSet* query_result)
{
	//nothing
}

