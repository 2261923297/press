#include "PressClient.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <thread>

namespace app {


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
	this->connect(addr);

	m_sem.reset(new thread::Semaphore(0));
}

PressClient::~PressClient() {
	this->close();
}

bool PressClient::connect(const char* addr)  {
	m_path.reset();
	m_path = net::Path::ptr(new net::Path(addr));
	
	m_sock = net::Socket::ptr(new net::Socket(AF_INET, SOCK_STREAM, 0));
	return m_sock->connect(*m_path);

}

bool PressClient::close()  {
	m_sock->shutdown();
	return true;
}


bool PressClient::set()  {
	int ret = m_sock->send(s_cmdSet, 5);
	ret &= m_sock->send(m_setPressVal, 3);
	return ret;
}

bool PressClient::get()  {
	return !!m_sock->send(s_cmdGet, 5);
}

void PressClient::notice() {
	m_sem->wait();
	printf("压力: 0x%u\n", get_press(GetPress(), 3));
}

void PressClient::reconnect(int n_times) {
	int n = n_times;
	while(n_times--) {
		if(-1 == m_sock->connect(*m_path)) {
			continue;
		} else {
			return;
		}
	}
	printf("重连 %d 次失败\n", n);
}

void thrd_wait(net::Socket::ptr m_sock, thread::Semaphore::ptr sem) {
	const size_t b_size = 8;
	char buffer[b_size] = { 0 };
	uint32_t n_recv;
	int notice_key = 1;

	while(1) {
		memset(buffer, 0, b_size);
		n_recv = m_sock->recv(buffer, b_size);

		if(-1 == n_recv) {
			printf("ERROR %d", __LINE__);
			return;
		}
		std::cout << "nRecv: " << n_recv << ": ";
		for(int i = 0; buffer[i] != 0; i++) {
			printf("%x ", buffer[i]);
		}
		printf("\n");
		if(n_recv == 3) {
			PressClient::SetPress(buffer);
			sem->post();						// 释放信号
		}
	}
	return;
}

bool PressClient::wait()  {
	std::thread thrd_recv_press(this, thrd_wait, m_sock, m_sem);
	thrd_recv_press.detach();

	return true;
}




} // namespace app
