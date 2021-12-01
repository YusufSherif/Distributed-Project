#ifndef UDPSERVERSOCKET_H
#define UDPSERVERSOCKET_H
#include "UdpSocket.h"

class UDPServerSocket : public UDPSocket
{
public:
	UDPServerSocket ();
	bool initializeServer (const char *_myAddr, int _myPort);
	~UDPServerSocket ( );
};
#endif // UDPSERVERSOCKET_H 