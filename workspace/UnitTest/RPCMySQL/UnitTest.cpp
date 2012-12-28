#include "UnitTest.h"

#include <string>
#include <sstream>

#include "Utils/CommandBase.h"

#include "Db/ResultSet.h"

#include "Net/Ctx.h"
#include "Net/MySQLRPCClient.h"
#include "Net/MySQLRPCServer.h"
#include "Net/IOThread.h"


TestRPCMysql::TestRPCMysql(void)
{

}

TestRPCMysql::~TestRPCMysql(void)
{

}

int TestRPCMysql::Run(void)
{
	FREEZE_NET::Ctx context(2);

	std::stringstream ss;
	ss << "DROP TABLE IF EXISTS `tb_role_farm`;";

	std::string table;
	std::string query_statement = ss.str();
	CommandBaseEx* callback_cmd = NULL;
	ResultSet* query_result = NULL;


	context.mysql_rpc_client_->SendRPC(context.mysql_rpc_server_,
			table.c_str(), query_statement.c_str(),
			callback_cmd, query_result);

	ss.str("");

	ss << "CREATE TABLE `tb_role_farm` ( "
		<< "`id` int(11) NOT NULL AUTO_INCREMENT, "
		<< "`role_id` int(11) NOT NULL DEFAULT '0', "
		<< "`farm_level` int(11) NOT NULL DEFAULT '1', "
		<< "`farm_cur_experience` int(11) NOT NULL DEFAULT '0', "
		<< "`today_add_experience` int(11) NOT NULL DEFAULT '0', "
		<< "`str_value` varchar(1024) NOT NULL DEFAULT '', "
		<< "PRIMARY KEY (`id`) "
		<< ") ENGINE=InnoDB AUTO_INCREMENT=323 DEFAULT CHARSET=utf8; ";

	query_statement = ss.str();
	context.mysql_rpc_client_->SendRPC(context.mysql_rpc_server_,
			table.c_str(), query_statement.c_str(),
			callback_cmd, query_result);

	ss.str("");

	ss << "INSERT INTO `tb_role_farm` VALUES ('1', '100004290', '1', '1', '1', 'qwert123'), ('2', '200004290', '3', '3', '3', '333333qwert123');";

	query_statement = ss.str();
	context.mysql_rpc_client_->SendRPC(context.mysql_rpc_server_,
			table.c_str(), query_statement.c_str(),
			callback_cmd, query_result);

	ss.str("");

	ss << "UPDATE `tb_role_farm` SET `farm_level`='2',`farm_cur_experience`='2',`today_add_experience`='2' WHERE (`id`='1') ";

	query_statement = ss.str();
	context.mysql_rpc_client_->SendRPC(context.mysql_rpc_server_,
			table.c_str(), query_statement.c_str(),
			callback_cmd, query_result);

	ss.str("");

	ss << "SELECT "
		<< "tb_role_farm.id, "
		<< "tb_role_farm.role_id, "
		<< "tb_role_farm.farm_level, "
		<< "tb_role_farm.farm_cur_experience, "
		<< "tb_role_farm.today_add_experience, "
		<< "tb_role_farm.str_value "
		<< "FROM "
		<< "tb_role_farm ";

	query_statement = ss.str();
	context.mysql_rpc_client_->SendRPC(context.mysql_rpc_server_,
			table.c_str(), query_statement.c_str(),
			callback_cmd, query_result);

	std::cout << "Enter any key to eixt !" << std::endl;
	std::string end;
	std::cin >> end;

	return 0;
}
