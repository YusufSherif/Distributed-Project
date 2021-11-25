//
// Created by Yusuf Sherif on 21/11/2021.
//

#include "../include/Client.h"

Client::Client(char *_hostname, int _port) {
	udpSocket = new UDPClientSocket();
	udpSocket->initializeClient(_hostname, _port);
}
Message Client::execute(Message *_message) {
	std::string msg = _message->marshal();
	char *res = new char[1000];
	udpSocket->writeToSocket(msg.c_str(), msg.length() + 1);
	udpSocket->readFromSocketWithBlock(res, msg.length() + 1);
	Message resultM = Message(10, res, msg.length() + 1, 0);
	delete[] res;
	return resultM;
}
Client::~Client() {
	delete udpSocket;
}
