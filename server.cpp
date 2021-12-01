#include <iostream>
#include "include/Client.h"
#include "include/Server.h"
#include <future>

int main() {
	Server s("10.40.57.52", 8000);
	while (true) {
		s.serveRequest();
		sleep(1);
	}
}
