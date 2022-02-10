#include "Semaphore.h"
#include <unistd.h>
#include <string.h>
#include <thread>

void creator(thread::Semaphore::ptr sem) {
	while(1) {
		sem->post();
		sleep(1);
	}
}

void consumer(thread::Semaphore::ptr sem) {
	while(1) {
		sem->wait();
		printf("get 1 semaphore\n");
	} 
	
}

void test_in_fiber() {


}

void test_in_thread() {
	thread::Semaphore::ptr sem(new thread::Semaphore);
	std::thread thrd_creator(creator, sem);
	std::thread thrd_consumer(consumer, sem);
	while(1) {
		sleep(1);
	}

}

int main() {
	test_in_thread();
	
	return 0;
}
