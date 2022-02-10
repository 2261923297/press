#include "Thread.h"


namespace thread {

Thread::Thread() {
	m_sem.reset(new Semaphore(0));
	m_thread = pthread_create(&m_thread, )




	m_sem->wait();
}

Thread::~Thread() {

}


} // namespace thread
