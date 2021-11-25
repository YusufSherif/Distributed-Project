#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <dns_util.h>
#include <cstring>
#include <unistd.h>

class UDPSocket
{
protected:
	int sock;
	sockaddr_in myAddr;
	sockaddr_in peerAddr;
	char * myAddress;
	char * peerAddress;
	int myPort;
	int peerPort;
	bool enabled;
	pthread_mutex_t mutex;
public:
	UDPSocket ();
	void setFilterAddress (char * _filterAddress);
	char * getFilterAddress ();
	virtual bool initializeServer (char * _myAddr, int _myPort);
	virtual bool initializeClient (char * _peerAddr, int _peerPort);
	int writeToSocket (const char * buffer, int maxBytes );
	int writeToSocketAndWait (char * buffer, int maxBytes,int microSec );
	int readFromSocketWithNoBlock (char * buffer, int maxBytes );
	int readFromSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, int timeoutMilli);
	int readFromSocketWithBlock (char * buffer, int maxBytes );
	int readSocketWithNoBlock (char * buffer, int maxBytes );
	int readSocketWithTimeout (char * buffer, int maxBytes, int timeoutSec, int timeoutMilli);
	int readSocketWithBlock (char * buffer, int maxBytes );
	int getMyPort () const;
	int getPeerPort () const;
	void enable();
	void disable();
	bool isEnabled() const;
	void lock();
	void unlock();
	int getSocketHandler() const;
	~UDPSocket ( );
};
#endif // UDPSOCKET_H 