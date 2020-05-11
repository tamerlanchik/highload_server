//
// Created by Ian on 16.02.2020.
//

#ifndef HIGHLOAD_SERVER_H
#define HIGHLOAD_SERVER_H
#include <string>
//#include <ThreadPool.h>
//#include "Connection.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <mutex>
#include "Conntection.h"

class Server {
public:
	Server(int cpu_limit, int thread_limit, std::string doc_root, int port=8001);
	~Server();
	int Run(std::string address = "0.0.0.0", int port = 8000);
private:
    void startAccept();

    void handleAccept(Connection::pointer new_connection, const boost::system::error_code &error);

	int _thread_limit;
	int _cpu_limit;
	std::string _doc_root;
	std::string _address;

//	ThreadPool _thread_pool;

    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::acceptor _acceptor;
    boost::thread_group _thread_pool;

//    std::vector<std:mutex> _mutex_pool;
//    boost::asio::thread_pool _thread_pool;

//	static void dispatchering_connection_cb(uv_stream_t* server, int status);

};

#endif //HIGHLOAD_SERVER_H
