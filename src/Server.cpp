//
// Created by Yusuf Sherif on 21/11/2021.
//

#include "../include/Server.h"

Message *Server::getRequest() {
	std::vector<char> buffer;
	static const size_t MaxBytesPerRecv = 1024;
	int bytesRead = 0;
	int i = 0;
	int totalSize = 0;
	do {
		const size_t oldSize = buffer.size();

		buffer.resize(oldSize + MaxBytesPerRecv);
		bytesRead =
		udpServerSocket->readFromSocketWithNoBlock(&(buffer.data())[(i) * (MaxBytesPerRecv)], MaxBytesPerRecv);
		std::cout << "Server: read the following from socket " + std::string(buffer.data()) << std::endl;
		if (bytesRead == -1) {
			std::cout << "Error occured" << std::endl;
			break;
		}
		i++;
		buffer.resize(oldSize + bytesRead);
		totalSize = oldSize + bytesRead;
	} while (bytesRead > 0);

	buffer.resize(totalSize + 1);
	buffer[totalSize+1] = '\0';
	if(totalSize<=0){
		return nullptr;
	}
	Message *resultM = new Message(buffer.data());
	std::cout << "Server: recieved message " + resultM->toStirng() << std::endl;
	return resultM;
}

void *Server::doOperation(Message *_message) {
	_message->setMessageType(Reply);
}

void Server::sendReply(Message *_message) {
	std::string msg = _message->marshal();
	udpServerSocket->writeToSocket(msg.data(), 1024);
	std::cout << "Sent Message: " + msg << std::endl;
}

Server::Server(const char *_listen_hostAddr, int _listen_port) {
	udpServerSocket = new UDPServerSocket();
	udpServerSocket->initializeServer(_listen_hostAddr, _listen_port);
}
void Server::serveRequest() {
	Message *m = getRequest();
	if (m == nullptr)
		return;
	doOperation(m);
	sendReply(m);
	delete m;
}
Server::~Server() {
	delete udpServerSocket;
}
