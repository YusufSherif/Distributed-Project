#ifndef UDPCLIENTSOCKET_H
#define UDPCLIENTSOCKET_H
#include "UdpSocket.h"

class UDPClientSocket : public UDPSocket
{
public:
	UDPClientSocket ();
	bool initializeClient (const char * _peerAddr, int _peerPort);
	~UDPClientSocket ( );
};
#endif // UDPCLIENTSOCKET_H