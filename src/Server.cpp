//
// Created by Ian on 16.02.2020.
//

#include <Server.h>
#include <unistd.h>
#include <NetWorker.h>

Server::Server(int cpu_limit, int thread_limit, std::string doc_root) :
		_cpu_limit(cpu_limit),
		_thread_limit(thread_limit),
		_doc_root(doc_root), _thread_pool(cpu_limit, thread_limit) {

}

Server::~Server() {}

void connection_cb(uv_stream_t * server, int status);

int Server::Run(std::string address, int port) {

	_thread_pool.Prepare();

	sockaddr_in addr = {0};

	//	inflate addr struct
	if(uv_ip4_addr(address.c_str(), port, &addr) != 0) {
		perror("Error in uv_ip4_addr");
		return -1;
	}
	uv_loop_t* loop = uv_default_loop();
	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	uv_tcp_bind(&server, (const sockaddr*)&addr, 0); // 0 = ipv4

	int r = uv_listen((uv_stream_t *) &server, 128, Server::dispatchering_connection_cb);
	if(r < 0) {
		perror("Cannot listen");
		return r;
	}

	uv_run(loop, UV_RUN_DEFAULT);
	uv_loop_close(loop);
	free(loop);
	printf("End server.Run()");
	return 0;
}

//	OnNewConnection callback for uv. Dispatches request for several workers
void Server::dispatchering_connection_cb(uv_stream_t* server, int status) {
	_thread_pool.AddTask()
}

std::function<int()> bind_task(uv_stream_t* server, ...) {
	return std::function<int()>
}
