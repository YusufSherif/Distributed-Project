#ifndef SERVER_H
#define SERVER_H
#include "Message.h"
#include "UdpServerSocket.h"

class Server
{
private:
	UDPServerSocket * udpServerSocket;
	Message * getRequest();
	Message * doOperation();
	void sendReply (Message * _message);
public:
	Server(char * _listen_hostname, int _listen_port);
	void serveRequest();
	~Server();
};
#endif // SERVER_H