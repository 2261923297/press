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

void HandleError::work(int errnum, const char* file_name, const char* func_name, uint32_t line) {
	char buffer[256] { 0 };
	sprintf(buffer, "[net_error] %s:%d::%s desc: %s\n"
			, file_name, line, func_name, strerror(errnum));
	fprintf(stdout, "%s", buffer);
	this->handleError();
}

Socket::Socket(int domain, int type, int protocol) {
	m_domain = domain;
	m_type = type;
	m_protocol = protocol;
	m_handleError = HandleError::ptr(new HandleError);
	m_sock = 0;
}

Socket::~Socket() {
	this->close();
}

void Socket::close() { 
	if(m_sock != 0)
		::close(m_sock); 
	m_sock = 0; 
}


void Socket::handleError(const char* func_name, int ret, int line) {
	if(-1 == ret) {
		m_handleError->work(errno, __FILE__, func_name, line);
	}
}

bool Socket::bind(const net::Path& p) {
	m_path = Path::ptr(new Path(p));

	struct sockaddr_in  si;
	si.sin_family = m_domain;
	si.sin_addr.s_addr = m_path->getAddr();
	si.sin_port = m_path->getPort();

	int ret = ::bind(m_sock, (struct sockaddr*)&si, sizeof(si));
	handleError(__FUNCTION__,  ret, __LINE__);

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
	
	handleError(__FUNCTION__,  ret, __LINE__);
	return  !!!ret;

}

bool Socket::listen(uint32_t n) {
	int ret = ::listen(m_sock, n);
	handleError(__FUNCTION__,  ret, __LINE__);
	return !!!ret;
}

uint32_t Socket::send(const void* buffer, uint32_t size) {
	uint32_t ret = ::send(m_sock, buffer, size, 0);
	handleError(__FUNCTION__,  ret, __LINE__);
	return ret;
}

uint32_t Socket::recv(void* buffer, uint32_t size) {
	uint32_t ret = ::recv(m_sock, buffer, size,0);
	handleError(__FUNCTION__,  ret, __LINE__);
	return ret;
}


bool Socket::reconnect(size_t n_times) {
	if(0 != m_sock) {
		this->close();
		m_sock = socket(m_domain, m_type, m_protocol);
	}

	struct sockaddr_in  si;
	si.sin_family = m_domain;
	si.sin_addr.s_addr = m_path->getAddr();
	si.sin_port = m_path->getPort();

	int ret = -1;
	while(n_times-- && -1 == ret)
		ret = ::connect(m_sock, (struct sockaddr*)&si, sizeof(si));
	return !!!ret;
}

void Socket::shutdown() {

} 
void Socket::onShutdown() {

}


} // namespace net
