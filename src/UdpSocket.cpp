//
// Created by Yusuf Sherif on 21/11/2021.
//

#include "../include/UdpSocket.h"

UDPSocket::UDPSocket() {
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
}
bool UDPSocket::initializeServer(const char *_myAddr, int _myPort) {
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddr.sin_port = htons(_myPort);
	myAddr.sin_family = AF_INET;
	int error = bind(sock, (struct sockaddr *)&myAddr, sizeof(myAddr));
	return error;
}
bool UDPSocket::initializeClient(const char *_peerAddr, int _peerPort) {
	bzero(&peerAddr, sizeof(peerAddr));
	peerAddr.sin_addr.s_addr = inet_addr(_peerAddr);
	peerAddr.sin_port = htons(_peerPort);
	peerAddr.sin_family = AF_INET;
//	if(connect(sock, (struct sockaddr *)&peerAddr, sizeof(peerAddr)) < 0)
//	{
//		printf("\n Error : Connect Failed \n");
//		exit(0);
//	}
	return true;
}
int UDPSocket::writeToSocket(const char *buffer, int maxBytes) {
	std::string buf(buffer);

	int numberOfBytesSent = sendto(sock, buf.c_str(), buf.length() + 1, 0, (sockaddr *)&peerAddr, sizeof(peerAddr));
	if (numberOfBytesSent == -1) {
		perror("Could not send bytes!");
		return -1;
	}
	return numberOfBytesSent;
}
int UDPSocket::writeToSocketAndWait(char *buffer, int maxBytes, int microSec) {
	int bytesSent = writeToSocket(buffer, maxBytes);
	usleep(microSec);
	return bytesSent;
}
int UDPSocket::readFromSocketWithNoBlock(char *buffer, int maxBytes) {
	socklen_t len = sizeof(peerAddr);
	int msglen = recvfrom(sock, buffer, maxBytes, MSG_DONTWAIT, (sockaddr *)&peerAddr, &len);
	return msglen;
}
int UDPSocket::readFromSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec, int timeoutMilli) {
	struct timeval tv;
	tv.tv_sec = timeoutSec;
	tv.tv_usec = timeoutMilli*1000;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	int bytes = readFromSocketWithBlock(buffer, maxBytes);
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
	return bytes;
}
int UDPSocket::readFromSocketWithBlock(char *buffer, int maxBytes) {
	socklen_t len = sizeof(peerAddr);
	int msglen = recvfrom(sock, buffer, maxBytes, 0, (sockaddr *)&peerAddr, &len);
	return msglen;
}

int UDPSocket::getMyPort() const {
	return myPort;
}
int UDPSocket::getPeerPort() const {
	return peerPort;
}
void UDPSocket::enable() {
	enabled = true;
}
void UDPSocket::disable() {
	enabled = false;
}
bool UDPSocket::isEnabled() const {
	return enabled;
}
void UDPSocket::lock() {
	pthread_mutex_lock(&mutex);
}
void UDPSocket::unlock() {
	pthread_mutex_unlock(&mutex);
}
int UDPSocket::getSocketHandler() const {
	return sock;
}
UDPSocket::~UDPSocket() = default;
