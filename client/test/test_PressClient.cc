#include "PressClient.h"
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace app;
using namespace net;

void thrd_send_cmd(PressClient* pc) {
	while(1) {
		sleep(1);
		pc->set();
	}
}

void thrd_recv_data(PressClient* pc) {
	pc->wait();
	pc->notice();
}	

int main() {

	std::cout << "hello!" << std::endl;
	PressClient pc("192.168.1.66");
	
	std::thread thrd_send(thrd_send_cmd, &pc);
	std::thread thrd_recv(thrd_recv_data, &pc);
	thrd_send.detach();
	thrd_recv.detach();

	while(1) {
		sleep(1);
	}
	return 0;
}
