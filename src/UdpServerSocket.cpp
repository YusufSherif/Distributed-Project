//
// Created by Yusuf Sherif on 21/11/2021.
//

#include "../include/UdpServerSocket.h"

UDPServerSocket::UDPServerSocket() {

}
bool UDPServerSocket::initializeServer(const char *_myAddr, int _myPort) {
	return UDPSocket::initializeServer(_myAddr, _myPort);
}
UDPServerSocket::~UDPServerSocket() {

}
