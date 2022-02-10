#include "Thread.h"

#include <iostream>
#include <functional>


int f_cosplay(int a, int b) {
	return a + b;
}


void test_changeArgs(args...) {
	
	std::cout << "cosPlay: " << f_cosplay(...args) << std::endl;

}

void test_functional() {
	test_changeArgs(3, 4);

}

int main() {
	test_functional();

	std::cout << "thread!" << std::endl;

	return 0;
}





