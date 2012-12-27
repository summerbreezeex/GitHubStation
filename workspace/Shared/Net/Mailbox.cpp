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

#include <assert.h>

#include "Mailbox.h"

NET::Mailbox::Mailbox(void)
{
}

NET::Mailbox::~Mailbox(void)
{
}

NET::fd_t NET::Mailbox::GetFd(void)
{
	return signaler_.GetFd();
}

void NET::Mailbox::Send(const Command& cmd)
{
	sync_.Lock();
	this->cpipe_.push_back(cmd);
	sync_.Unlock();

	signaler_.Send();
}

int NET::Mailbox::Recv(Command* cmd)
{
	signaler_.Recv();

	sync_.Lock();
	if (!this->cpipe_.empty())
	{

		*cmd = this->cpipe_.front();
		this->cpipe_.pop_front();

		sync_.Unlock();
		return 0;
	}

	sync_.Unlock();
	return -1;

}

