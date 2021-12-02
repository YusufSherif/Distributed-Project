#include "include/Client.h"

int main(){
	Client c("127.0.0.1", 8000);
	while (true) {
		Message m(0, "Hello World", strlen("Hello World") + 1, 0);
		Message* revd = c.execute(&m);
		delete revd;
		sleep(1);
	}
}

/*
 * We tested the reliabilty of datagrams. we noticed that datagrams are dropped if there is congestion while sending requests
 */