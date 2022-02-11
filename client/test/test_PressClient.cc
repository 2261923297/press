#include "PressClient.h"
#include "util.h"
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace app;
using namespace net;

void thrd_send(PressClient* pc) {
	while(1) {
		sleep(1);
		if(-1 == pc->set()) {
			pc->reconnect(3);
		}
	}
}

void thrd_wait(PressClient* pc) {
	while(1) {
		if(-1 == pc->wait()) {
			sleep(1);
		}

	}	
}

void thrd_notice(PressClient* pc) {
	while(1)
		pc->notice();
}	

int main() {
	std::cout << "hello!" << std::endl;

	PressClient pc("192.168.1.66");
	
	std::thread t_send(thrd_send, &pc);
	std::thread t_wait(thrd_wait, &pc);
	std::thread t_notice(thrd_notice, &pc);
	
	while(1) {
		sleep(1);
	}
	return 0;
}
