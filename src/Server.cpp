//
// Created by Ian on 16.02.2020.
//

#include <Server.h>
#include <unistd.h>
#include <NetWorker.h>
#include <boost/asio.hpp>
#include <iostream>

Server::Server(int cpu_limit, int thread_limit, std::string doc_root, int port) :
		_cpu_limit(cpu_limit),
		_thread_limit(thread_limit),
		_doc_root(doc_root),
        _acceptor(_io_service,  boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port)) {
//        _thread_pool(thread_limit) {
    _acceptor.non_blocking(true);
}

Server::~Server() {}

//void connection_cb(uv_stream_t * server, int status);

int Server::Run(std::string address, int port) {
    for (std::size_t i = 0; i < _thread_limit; i++) {
        _thread_pool.create_thread(boost::bind(&boost::asio::io_service::run, &_io_service));
        _io_service.post(boost::bind(&Server::startAccept, this));  // assing task for threadpool
    }
    _thread_pool.join_all();

//	_thread_pool.Prepare();
//
//	sockaddr_in addr = {0};
//
//	//	inflate addr struct
//	if(uv_ip4_addr(address.c_str(), port, &addr) != 0) {
//		perror("Error in uv_ip4_addr");
//		return -1;
//	}
//	uv_loop_t* loop = uv_default_loop();
//	uv_tcp_t server;
//	uv_tcp_init(loop, &server);
//
//	uv_tcp_bind(&server, (const sockaddr*)&addr, 0); // 0 = ipv4
//
//	int r = uv_listen((uv_stream_t *) &server, 128, Server::dispatchering_connection_cb);
//	if(r < 0) {
//		perror("Cannot listen");
//		return r;
//	}
//
//	uv_run(loop, UV_RUN_DEFAULT);
//	uv_loop_close(loop);
//	free(loop);
//	printf("End server.Run()");
	return 0;
}

//	OnNewConnection callback for uv. Dispatches request for several workers
//void Server::dispatchering_connection_cb(uv_stream_t* server, int status) {
//	_thread_pool.AddTask()
//}
//
//std::function<int()> bind_task(uv_stream_t* server, ...) {
//	return std::function<int()>
//}

void Server::startAccept() {
    Connection::pointer newConnection(new Connection(_io_service, _doc_root));
//    boost::this_thread::get_id() uncomment this to see how many threads
//    std::cerr << "Start accept()\n";
    // accept new connecton
    // returns
    _acceptor.async_accept(
            newConnection->getSocket(),
            // on-accept callback
            boost::bind(&Server::handleAccept, this, newConnection, boost::asio::placeholders::error)
    );
}

void Server::handleAccept(Connection::pointer new_connection, const boost::system::error_code &error) {
//    std::cerr << "handle accept()\n";
    if (!_acceptor.is_open()) {
        return;
    }

    if (!error) {
        std::cerr << "handle accept() -> startConnections()\n";
        new_connection->start();
    }

//    std::cerr << error.message() << ": handle accept: end()\n";

    startAccept();
}