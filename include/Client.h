#include <memory>

namespace app {

class Client {
public:
	Client() { }
	virtual ~Client() { }

	virtual bool connect(const char* addr) = 0;
	virtual bool close() = 0;

	virtual bool set() = 0;
	virtual bool get() = 0;
	virtual bool wait() = 0;

private:
	
}; // Client;




} // namespace tt
