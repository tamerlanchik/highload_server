//
// Created by Ian on 16.02.2020.
//

#ifndef HIGHLOAD_SERVER_H
#define HIGHLOAD_SERVER_H
#include <string>
#include <ThreadPool.h>

class Server {
public:
	Server(int cpu_limit, int thread_limit, std::string doc_root);
	~Server();
	int Run(std::string address, int port);
private:
	int _thread_limit;
	int _cpu_limit;
	std::string _doc_root;
	std::string _address;

	ThreadPool _thread_pool;

	static void dispatchering_connection_cb(uv_stream_t* server, int status);

};

#endif //HIGHLOAD_SERVER_H
