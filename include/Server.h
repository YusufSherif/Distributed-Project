#ifndef SERVER_H
#define SERVER_H
#include "Message.h"
#include "UdpServerSocket.h"
#include <vector>
#include <iostream>

class Server
{
private:
	UDPServerSocket * udpServerSocket;
	Message * getRequest();
	void *doOperation(Message *_message);
	void sendReply (Message * _message);
public:
	Server(const char * _listen_hostAddr, int _listen_port);
	void serveRequest();
	~Server();
};
#endif // SERVER_H