#ifndef CLIENT_H
#define CLIENT_H
#include "UdpClientSocket.h"
#include "Message.h"
#include <vector>
#include <iostream>

class Client
{
private:
	UDPClientSocket * udpSocket;
public:
	Client(const char * _hostAddr, int _port);
	Message* execute(Message * _message);
	~Client();
};
#endif // CLIENT_H