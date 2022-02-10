#pragma once

#include <stdint.h>
#include <memory>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace net {

class Path {
public:
	typedef std::shared_ptr<Path> ptr;
	Path(const std::string& path_str, uint16_t port = 8080);
	~Path() { }

	in_addr_t getAddr() const { return inet_addr(m_pathStr.c_str()); }
	uint16_t getPort() const { return htons(m_port); }

private:

	std::string m_pathStr;
	uint16_t m_port;
}; // class Path;

class Socket {
public:
	typedef std::shared_ptr<Socket> ptr;
	Socket(int domain, int type, int protocol);
	virtual ~Socket();
	void close() { ::close(m_sock); m_sock = 0; }

	bool bind(const net::Path& p);
	bool connect(const net::Path& p);
	Socket* accept();
	void listen(uint32_t n);
	uint32_t send(const void* buffer, uint32_t size);
	uint32_t recv(void* buffer, uint32_t size);
	
	void shutdown();    // send empty packet to shutdown
	void onShutdown();  // recv empty packet call this func to sure;

private:

	int m_domain;
	int m_type;
	int m_protocol;

	net::Path::ptr m_path;
	int m_sock;

}; //class Socket;


} // namespace net


