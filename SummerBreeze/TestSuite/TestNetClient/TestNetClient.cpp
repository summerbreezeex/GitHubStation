#include "TestNetClient.h"


#include "../../SharedLib/Shared/Net/zmq_listener.hpp"
#include "../../SharedLib/Shared/Net/concrete_protocol.hpp"
using namespace zmq;

#include <iostream>
#include <sstream>

TestNetClient::TestNetClient(void)
{

}

TestNetClient::~TestNetClient(void)
{

}
int TestNetClient::Run(void)
{
	protocol_binary_t::add_prototype_instance();

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

		std::stringstream ss;
		ss << "*" << count << "*";


		protocol_test_t * ptr_protocol = new protocol_test_t(); 
		ptr_protocol->data = ss.str();

		PackPacket pack_packet;
		pack_packet << ptr_protocol;

		int nSend = ::send(s, pack_packet.Buf(), pack_packet.Length(), 0);

		int nReceive;
		char buf[1024];

		nReceive = ::recv(s, buf, sizeof(buf), 0);
		if (nReceive == -1)
		{
			return 0;
		}
		else if (nReceive == 0)
		{
			break;
		}
		else
		{
			try 
			{
				PackPacket pack_packet_1;
				pack_packet_1.Append(buf, nReceive);

				uint32_t length = pack_packet_1.Length();
				if (length >= 20)
				{
					uint32_t packet_length = *((uint32_t*)(pack_packet.Buf() + 4));

					if (length >= packet_length)
					{
						char* ptr = pack_packet.Drain(packet_length);
						if (NULL == ptr)
						{
							return 0;
						}

						uint32_t opcode = *((uint32_t*)(ptr + 12));

						UnpackPacket unpack_packet(ptr, packet_length);

						protocol_binary_t* protocol_base = protocol_binary_t::find_and_clone(opcode);
						if (protocol_base != NULL)
						{
							unpack_packet >> protocol_base;
							protocol_base->run();
							delete protocol_base;
							protocol_base = NULL;
						}
						else
						{
							delete ptr;
							ptr = NULL;
							return 0;
						}

						delete ptr;
						ptr = NULL;
					}

				}
			}
			catch( std::exception& e )
			{
				std::cout << e.what() << std::endl;
				return 0;
			}
			catch(...)
			{
				std::cout << "catch ..." << std::endl;
				return 0;
			}
		}

		Sleep(20);

		// �ر��׽���
		::closesocket(s);
	}

	system("pause");

	return 0;
}