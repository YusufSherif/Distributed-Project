#ifndef SERVER_H
#define SERVER_H
#include "Message.h"
#include "UdpServerSocket.h"
#include "Database.h"
#include "../libs/nlohmann_json/json.hpp"
#include "../libs/CppSteg/include/CppSteg/LeastBit.hpp"
#include "imageOperations.hpp"
#include <vector>
#include <iostream>


class Server
{
private:
	Database db;
	UDPServerSocket * udpServerSocket;
	Message getRequest();
	Message doOperation(Message _message);
	void sendReply (Message _message);
public:
	Server(const char *_listen_hostAddr, int _listen_port, const std::string &db_path);
	void serveRequest();
	~Server();
};
#endif // SERVER_H