#include "TestNetClient.h"


#include "../../SharedLib/Shared/Net/zmq_listener.hpp"
#include "../../SharedLib/Shared/Net/concrete_protocol.hpp"
using namespace zmq;

TestNetClient::TestNetClient(void)
{

}

TestNetClient::~TestNetClient(void)
{

}
int TestNetClient::Run(void)
{
	int count = 100000;
	while(count--)
	{
		//Sleep(1);
		// 创建套节字
		SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(s == INVALID_SOCKET)
		{
			printf(" Failed socket() \n");
			return 0;
		}

		// 也可以在这里调用bind函数绑定一个本地地址
		// 否则系统将会自动安排

		// 填写远程地址信息
		sockaddr_in servAddr; 
		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(4567);//htons(4567);

		// 注意，这里要填写服务器程序（TCPServer程序）所在机器的IP地址
		// 如果你的计算机没有联网，直接使用127.0.0.1即可
		servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		//servAddr.sin_addr.s_addr
		if(::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
		{
			printf(" Failed connect() (%ld)\n",WSAGetLastError());

			return 0;
		}

		// 接收数据
		//char buff[256];
		//std::string str_buff = "123456789qwertyuio";
		//memcpy(buff, str_buff.c_str(), str_buff.size()+1);
		//int nSend = ::send(s, buff, strlen(buff) + 1, 0);

		protocol_binary_t * ptr_protocol = new protocol_test_t(); 
		PackPacket pack_packet;
		pack_packet << ptr_protocol;

		int nSend = ::send(s, pack_packet.Buf(), pack_packet.Length(), 0);

		Sleep(20);

		// 关闭套节字
		::closesocket(s);
	}

	system("pause");

	return 0;
}