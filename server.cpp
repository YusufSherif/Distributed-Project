#include <iostream>
#include "include/Server.h"
#include <future>

int main() {
	Server s("127.0.0.1", 8000, "db.sqlite3");
	while (true) {
		s.serveRequest();
		sleep(1);
	}
}
