#include <semaphore.h>
#include <memory>

namespace thread {
class Semaphore {
public:
	typedef std::shared_ptr<thread::Semaphore> ptr;
	Semaphore(int n_sem = 0);
	~Semaphore();

	void post();
	void wait();
	void destroy();



private:
	sem_t m_semaphore;


}; // class Semaphore



} // namespace thread
