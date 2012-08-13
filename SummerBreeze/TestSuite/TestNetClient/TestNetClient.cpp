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
		// �����׽���
		SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(s == INVALID_SOCKET)
		{
			printf(" Failed socket() \n");
			return 0;
		}

		// Ҳ�������������bind������һ�����ص�ַ
		// ����ϵͳ�����Զ�����

		// ��дԶ�̵�ַ��Ϣ
		sockaddr_in servAddr; 
		servAddr.sin_family = AF_INET;
		servAddr.sin_port = htons(4567);//htons(4567);

		// ע�⣬����Ҫ��д����������TCPServer�������ڻ�����IP��ַ
		// �����ļ����û��������ֱ��ʹ��127.0.0.1����
		servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		//servAddr.sin_addr.s_addr
		if(::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
		{
			printf(" Failed connect() (%ld)\n",WSAGetLastError());

			return 0;
		}

		// ��������
		//char buff[256];
		//std::string str_buff = "123456789qwertyuio";
		//memcpy(buff, str_buff.c_str(), str_buff.size()+1);
		//int nSend = ::send(s, buff, strlen(buff) + 1, 0);

		protocol_binary_t * ptr_protocol = new protocol_test_t(); 
		PackPacket pack_packet;
		pack_packet << ptr_protocol;

		int nSend = ::send(s, pack_packet.Buf(), pack_packet.Length(), 0);

		Sleep(20);

		// �ر��׽���
		::closesocket(s);
	}

	system("pause");

	return 0;
}