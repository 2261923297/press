#include "Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

static char g_error_buffer[256] = { 0 };
#define print_err(val) \
	do {									\
		if(val <= 0) {						\
			memset(g_error_buffer, 0, 256);		\
			sprintf(g_error_buffer, "[ERROR] %d, %d: %s\n", errno, __LINE__, strerror(errno));	 \
			printf("%s", g_error_buffer);		\
		}									\
	} while(0)
	
namespace net {

Path::Path(const std::string& path_str, uint16_t port) { 
	m_pathStr = path_str; 
	m_port = port;
}
Socket::Socket(int domain, int type, int protocol) {
	m_domain = domain;
	m_type = type;
	m_protocol = protocol;
	m_sock = 0;
}

Socket::~Socket() {

}

void Socket::close() { 
	if(m_sock != 0)
		::close(m_sock); 
	m_sock = 0; 

}
bool Socket::bind(const net::Path& p) {

	m_path = Path::ptr(new Path(p));

	struct sockaddr_in  si;
	si.sin_family = m_domain;
	si.sin_addr.s_addr = m_path->getAddr();
	si.sin_port = m_path->getPort();

	int ret = ::bind(m_sock, (struct sockaddr*)&si, sizeof(si));
	if(-1 == ret) {
		print_err(errno);		
	}
	return  !!!ret;

}

Socket* Socket::accept() {
	Socket* s = nullptr; 


	return nullptr;
}

bool Socket::connect(const net::Path& p) {
	m_path = Path::ptr(new Path(p));

	if(0 != m_sock) {
		this->close();
		m_sock = socket(m_domain, m_type, m_protocol);
	}
	struct sockaddr_in  si;
	si.sin_family = m_domain;
	si.sin_addr.s_addr = m_path->getAddr();
	si.sin_port = m_path->getPort();

	int ret = ::connect(m_sock, (struct sockaddr*)&si, sizeof(si));
	if(-1 == ret) {
		print_err(errno);		
	}
	printf("ret = %d\n", ret);
	return  !!!ret;

}

void Socket::listen(uint32_t n) {
	int ret = ::listen(m_sock, n);
	print_err(ret);

}

uint32_t Socket::send(const void* buffer, uint32_t size) {
	uint32_t ret = ::send(m_sock, buffer, size, 0);
	printf("Socket::send = %d\n", ret);
	print_err(ret);

	return ret;
}

uint32_t Socket::recv(void* buffer, uint32_t size) {
	uint32_t ret = ::recv(m_sock, buffer, size,0);
	printf("Socket::recv = %d\n", ret);
	print_err(ret);

	return ret;
}

void Socket::shutdown() {

}

void Socket::onShutdown() {

}


} // namespace net
