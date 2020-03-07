//
// Created by Ian on 16.02.2020.
//

#ifndef HIGHLOAD_THREADPOOL_H
#define HIGHLOAD_THREADPOOL_H
#include <uv.h>
#include <vector>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>


typedef std::function<int(uv_stream_t*)> task_t;
// Possesses a pool of threads
// and dispatches tasks between them
class ThreadPool {
public:
	ThreadPool(int cpu_limit, int thread_limit);

	int Prepare();

	int AddTask(task_t task);

private:
	std::mutex _queue_mutex;
	std::vector<std::thread> _pool;
	std::queue<task_t> _queue;
	int _cpu_limit;
	int _thread_limit;

	static int queue_server();
};

#endif //HIGHLOAD_THREADPOOL_H
