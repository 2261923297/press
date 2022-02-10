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

	const std::string& getStr() const { return m_pathStr; }
private:

	std::string m_pathStr;
	uint16_t m_port;
}; // class Path;

class HandleError {
	public:
	typedef std::shared_ptr<HandleError> ptr;
	HandleError() { }
	virtual ~HandleError() { }

	void work(int errnum, const char* file_name, const char* func_name, uint32_t line);

protected:
	virtual void handleError() { }

private:

}; // class HandleError


class Socket {
public:
	typedef std::shared_ptr<Socket> ptr;
	Socket(int domain, int type, int protocol);
	virtual ~Socket();
	void close();

	bool bind(const net::Path& p);
	bool connect(const net::Path& p);
	bool reconnect(size_t size);
	uint32_t send(const void* buffer, uint32_t size);
	uint32_t recv(void* buffer, uint32_t size);
	
	Socket* accept();
	bool listen(uint32_t n);

	void setHandleError(HandleError::ptr he) { m_handleError = he; }

	void shutdown();    // send empty packet to shutdown
	void onShutdown();  // recv empty packet call this func to sure;
protected:
	void handleError(const char* func_name, int ret, int line);

private:

	int m_domain;
	int m_type;
	int m_protocol;

	net::Path::ptr m_path;
	int m_sock;
	HandleError::ptr m_handleError;

}; //class Socket;


} // namespace net


