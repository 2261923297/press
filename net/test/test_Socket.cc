#include "Socket.h"
#include <iostream>
#include <unistd.h>

using namespace net;

void client() {
	Path p("192.168.1.66", 8080);
	Socket s(AF_INET, SOCK_STREAM, 0);
	s.connect(p);

	std::string cmd = "hello";
	while(1) {
		sleep(1);
		s.send(cmd.c_str(), cmd.size());
	}
	
}



int main() {
	client();

	return 0;
}




