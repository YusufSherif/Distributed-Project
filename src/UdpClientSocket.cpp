//
// Created by Yusuf Sherif on 21/11/2021.
//

#include "../include/UdpClientSocket.h"

UDPClientSocket::UDPClientSocket() {

}
bool UDPClientSocket::initializeClient(const char *_peerAddr, int _peerPort) {
	return UDPSocket::initializeClient(_peerAddr, _peerPort);
}
UDPClientSocket::~UDPClientSocket() {

}
