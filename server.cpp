#include <iostream>
#include "include/Client.h"
#include "include/Server.h"
#include <future>

int main() {
	Server s("127.0.0.1", 8000);
	while (true) {
		s.serveRequest();
		sleep(1);
	}
}
