#include "Semaphore.h"

namespace thread {

Semaphore::Semaphore(int n_sem) {
	sem_init(&m_semaphore, 0, n_sem);
}

Semaphore::~Semaphore() {
	destroy();
}

void Semaphore::post() {
	sem_post(&m_semaphore);
}

void Semaphore::wait() {
	sem_wait(&m_semaphore);
}

void Semaphore::destroy() {
	sem_destroy(&m_semaphore);
}



} // namespace thread
