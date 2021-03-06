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

#ifndef __SHARED_NET_COMMAND_H__
#define __SHARED_NET_COMMAND_H__

#include <stdint.h>
#include <string>

#include "../Utils/CommandBase.h"

class ResultSet;
class CommandBaseEx;

namespace FREEZE_NET
{

	//  This structure defines the commands that can be sent between threads.


	class Object;
	class MySQLRPCClient;

	struct Command
	{
		//  Object to process the command.
		Object* destination_;

		enum CMD_TYPE
		{
			STOP               = 0,
			RPC_MYSQL_REQUEST  = 1,
			RPC_MYSQL_RESPONSE = 2,
		} type_;

		union
		{
			struct
			{
				//uint32_t type_; //Must be zero (integer). Zero means that this message is the "Request" message
				uint32_t msg_id_;
				const char* table_;
				const char* query_statement_;
				CommandBaseEx* callback_cmd_;
				ResultSet* query_result_;
				MySQLRPCClient* request_object_;
			} RPC_MYSQL_REQUEST;

			struct
			{
				//uint32_t type_; //Must be one (integer). One means that this message is the "Response" message
				uint32_t msg_id_;
				uint32_t return_result_;
				ResultSet* query_result_;
			} RPC_MYSQL_RESPONSE;

            struct
            {
            } STOP;

		} args_;
	};

	//  Function to deallocate dynamically allocated components of the command.
	void DeallocateCommand(Command* cmd);

}

#endif
