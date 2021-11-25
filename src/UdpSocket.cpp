//
// Created by Yusuf Sherif on 21/11/2021.
//

#include "../include/UdpSocket.h"

void sighandler(int signum) {
	printf("Caught signal %d, coming out...\n", signum);
	throw std::runtime_error(std::to_string(signum));
}

UDPSocket::UDPSocket() {
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
}
void UDPSocket::setFilterAddress(char *_filterAddress) {

}
char *UDPSocket::getFilterAddress() {
	return nullptr;
}
bool UDPSocket::initializeServer(char *_myAddr, int _myPort) {
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(_myPort);
	inet_aton(_myAddr, &myAddr.sin_addr);
	bool error = bind(sock, (struct sockaddr *)&myAddr, sizeof(myAddr));
	return error;
}
bool UDPSocket::initializeClient(char *_peerAddr, int _peerPort) {
	peerAddr.sin_family = AF_INET;
	peerAddr.sin_port = htons(_peerPort);
	inet_aton(_peerAddr, &peerAddr.sin_addr);
	bool error = bind(sock, (struct sockaddr *)&peerAddr, sizeof(_peerAddr));
	return error;
}
int UDPSocket::writeToSocket(const char *buffer, int maxBytes) {
	size_t stringLength = strlen(buffer) + 1;
	std::string buf(buffer);
	int totalBytesSent = 0;
	for (size_t i = 0; i < stringLength; i += maxBytes) {
		if (i + maxBytes > stringLength) maxBytes = stringLength - i;
		std::string toSend = buf.substr(i, maxBytes);
		int numberOfBytesSent = sendto(sock, buf.c_str(), buf.length() + 1, 0, (sockaddr *)&peerAddr, sizeof(peerAddr));
		if (numberOfBytesSent == -1) {
			perror("Could not send bytes!");
			return -1;
		}
		totalBytesSent += numberOfBytesSent;
	}
	return totalBytesSent;
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
	try {
		socklen_t len = sizeof(peerAddr);
		signal(SIGALRM, sighandler);
		ualarm(timeoutMilli * 1000 + timeoutSec * 1000000, 0);
		int bytes = recvfrom(sock, buffer, maxBytes, 0, (sockaddr *)&peerAddr, &len);
		if (bytes < 0)
			if (errno == EINTR) {
				/* timed out */
			} else {
				/* some other error */
			}
		else
			/* no error or time out
			- turn off alarm */
			alarm(0);
		return bytes;
	} catch (std::runtime_error &e) {
		return -1;
	}
	return 0;
}
int UDPSocket::readFromSocketWithBlock(char *buffer, int maxBytes) {
	socklen_t len = sizeof(peerAddr);
	int msglen = recvfrom(sock, buffer, maxBytes, 0, (sockaddr *)&peerAddr, &len);
	return msglen;
}
int UDPSocket::readSocketWithNoBlock(char *buffer, int maxBytes) {
	return 0;
}
int UDPSocket::readSocketWithTimeout(char *buffer, int maxBytes, int timeoutSec, int timeoutMilli) {
	return 0;
}
int UDPSocket::readSocketWithBlock(char *buffer, int maxBytes) {
	return 0;
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
