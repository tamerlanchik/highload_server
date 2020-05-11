//
// Created by Ian on 16.02.2020.
//

#include <Server.h>
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


int Server::Run(std::string address, int port) {
    for (std::size_t i = 0; i < _thread_limit; i++) {
        _thread_pool.create_thread(boost::bind(&boost::asio::io_service::run, &_io_service));
        _io_service.post(boost::bind(&Server::startAccept, this));  // assing task for threadpool
    }
    _thread_pool.join_all();
	return 0;
}

void Server::startAccept() {
    Connection::pointer newConnection(new Connection(_io_service, _doc_root));
    _acceptor.async_accept(
            newConnection->getSocket(),
            // on-accept callback
            boost::bind(&Server::handleAccept, this, newConnection, boost::asio::placeholders::error)
    );
}

void Server::handleAccept(Connection::pointer new_connection, const boost::system::error_code &error) {
    if (!_acceptor.is_open()) {
        return;
    }

    if (!error) {
//        std::cerr << "handle accept() -> startConnections()\n";
        new_connection->start();
    }

    startAccept();
}