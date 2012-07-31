#include "../Utils/Utils.h"

#ifndef __SHARED_MYSQL_MYSQL_CONNECTION_H__
#define __SHARED_MYSQL_MYSQL_CONNECTION_H__

#include <string>

#include "RecordSet.h"

struct MySQLConnectionInfo
{
	std::string host;
	std::string user;
	std::string password;
	std::string database;
	unsigned int port;
};

class MySQLConnection
{
    public:
        MySQLConnection(void);
        virtual ~MySQLConnection(void);

		void Initialize(MySQLConnectionInfo& conn_info);

        virtual bool Connect(void);
        void Disconnect(void);

    public:

        bool Query(const char* sql, RecordSet* & ref_ptr_set, bool commit_flags = true);
     
        uint32_t GetLastError(void) 
		{ 
			return mysql_errno(this->mysql_); 
		}

		MYSQL* GetHandle(void)
		{
			return this->mysql_; 
		}

    private:
        bool HandleMySQLErrno(uint32_t err_no);
		bool CanTryReconnect(void);

    private:
        MYSQL *               mysql_;                    
        MySQLConnectionInfo  connection_info_;
		uint32_t  re_connect_count_;
};

#endif

