#include <pthread.h>
#include <string>
#include <stdint.h>
#include "Semaphore.h"

namespace thread {

class Semaphore;

class Thread {
public:

	Thread();
	~Thread();


private:
	void* run();


private:
	uint32_t m_id;
	std::string m_name;

	Semaphore::ptr  m_sem;
	pthread_t m_thread;

}; // class Thread



} // namespace thread
