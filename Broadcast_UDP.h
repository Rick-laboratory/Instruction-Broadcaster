#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#pragma comment(lib,"Ws2_32.lib")
#include<WinSock2.h>
#include <iostream>
#include<string>
class Broadcast_UDP
{
public:
	Broadcast_UDP();
	~Broadcast_UDP();
	void Broadcast(std::string);
private:
	WSADATA m_wsa;
	SOCKET m_socket;
	struct sockaddr_in m_send_addr;
	int m_error = 0;
	char m_broadcast = '1';
};