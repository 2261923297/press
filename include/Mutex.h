#pragma once 

namespace thread {
class Mutex {
	public:
		Mutex() { 
			pthread_mutex_init(&m_mutex, NULL);
			lock();
		}
		~Mutex() { 
			unlock();
			pthread_mutex_destroy(&m_mutex);
		}

		void lock() { 
			pthread_mutex_lock(&m_mutex);
		}
		void unlock() {
			pthread_mutex_unlock(&m_mutex);
		}

	private:
		pthread_mutex_t m_mutex;
}; // class Mutex

} // namespace thread
