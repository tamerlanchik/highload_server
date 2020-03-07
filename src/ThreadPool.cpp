#include <ThreadPool.h>

ThreadPool::ThreadPool(int cpu_limit, int thread_limit) :
		_cpu_limit(cpu_limit), _thread_limit(thread_limit){

}

int ThreadPool::Prepare() {
	_pool.reserve(_thread_limit);
	for(int i = 0; i < _thread_limit; i++) {
		_pool.emplace_back(ThreadPool::queue_server);
	}
}

int ThreadPool::queue_server() {
	_queue_mutex.lock();
	task_t task = _queue.pop();
	_queue_mutex.unlock();
	task();
}
