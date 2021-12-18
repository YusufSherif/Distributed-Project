//
// Created by Yusuf Sherif on 21/11/2021.
//

#include "../include/Client.h"

Client::Client(const char *_hostAddr, int _port) {
	udpSocket = new UDPClientSocket();
	udpSocket->initializeClient(_hostAddr, _port);
}
Message Client::execute(Message& _message) {
	std::string msg = _message.marshal();
	std::cout << "Client: writing " + msg + " to udp socket" << std::endl;
	udpSocket->writeToSocket(msg.c_str(), 1024);
	std::cout << "Client: finished writing" << std::endl;

	std::vector<char> buffer;
	static const size_t MaxBytesPerRecv = 1024;
	int bytesRead;
	int i = 0;
	int totalSize = 0;
	do {
		const size_t oldSize = buffer.size();

		buffer.resize(oldSize + MaxBytesPerRecv);
		bytesRead = udpSocket->readFromSocketWithTimeout(&(buffer.data())[(i) * (MaxBytesPerRecv)], MaxBytesPerRecv,5,0);
		std::cout << "Client: read from socket" << std::endl;

		i++;
		totalSize = oldSize + bytesRead;
	} while (bytesRead > 0);

	buffer.resize(totalSize + 1);
	buffer[totalSize+1] = '\0';
	if(totalSize<=0){
		return Message();
	}

	Message resultM(buffer.data());
	std::cout << "Client: read from socket the following payload " + resultM.toStirng() << std::endl;

	return resultM;
}
Client::~Client() {
	delete udpSocket;
}
