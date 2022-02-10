#include "PressClient.h"
#include "util.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>

namespace app {

void PressHandleError::handleError() {
	printf("handle error\n");
	m_pc->reconnect();
}

uint32_t get_press(const void* buffer, uint32_t size) { 
	uint32_t rt = 0;
	char* char_rt = (char*)&rt;
	for(int i = 0; i < size; i++) {
		char_rt[size - 1 -i] = ((char*)buffer)[i];
	}
	return rt;
}

PressClient::PressClient(const char* addr) {
	m_path.reset();
	m_sock.reset();
	m_handleError = PressHandleError::ptr(new PressHandleError(this));
	if(-1 == this->connect(addr)) {
		reconnect(3);
	}

	m_sem.reset(new thread::Semaphore(0));
}

PressClient::~PressClient() {
	this->close();
}

bool PressClient::connect(const char* addr)  {
	m_path.reset();
	m_path = net::Path::ptr(new net::Path(addr));
	
	m_sock = net::Socket::ptr(new net::Socket(AF_INET, SOCK_STREAM, 0));
	m_sock->setHandleError(m_handleError);
	int ret = m_sock->connect(*m_path);
	printf("ret = %d", ret);
	return ret;

}

bool PressClient::close()  {
	m_sock->shutdown();
	return true;
}


bool PressClient::set()  {
	char send[8] = { 0 };
	memcpy(send, s_cmdSet, 5);
	memcpy(send + 5, m_setPressVal, 3);
	int ret = m_sock->send(send, 8);
	if(-1 == ret) {
		reconnect(3);
	}
	return ret;
}

bool PressClient::get()  {
	return !!m_sock->send(s_cmdGet, 5);
}

void PressClient::notice() {
	while(1) { 
		m_sem->wait();
		usleep(5);
		printf("压力: 0x%u\n", get_press(GetPress(), 3));
	}

}

bool PressClient::reconnect(int n_times) {
	printf("reconnect... \n");
	int n = n_times;
	m_sock->close();

	m_sock->reconnect(n_times);

	printf("重连 %d 次失败\n", n);
	return false;
}

bool PressClient::wait()  {
	const size_t b_size = 8;
	char buffer[b_size] = { 0 };
	uint32_t n_recv;
	int notice_key = 1;

	while(1) {
		memset(buffer, 0, b_size);
		n_recv = m_sock->recv(buffer, b_size);

		std::cout << "nRecv: " << n_recv << ": ";
		for(int i = 0; buffer[i] != 0; i++) {
			printf("%x ", buffer[i]);
		}
		printf("\n");
		if(n_recv == 3) {
			PressClient::SetPress(buffer);
			m_sem->post();						// 释放信号
		}
	}

	return true;
}




} // namespace app
