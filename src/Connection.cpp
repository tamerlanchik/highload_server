//
// Created by andrey on 11.05.2020.
//

#include <Conntection.h>
#include <boost/thread/thread.hpp>
#include <iostream>

Connection::Connection(boost::asio::io_service &io_service, std::string rootDir):
    socket(io_service), request(rootDir), offset(0) {}

Connection::~Connection() {
    stop();
}

void Connection::stop() {
    if (socket.is_open()) {
        socket.close();
    }
}

void Connection::start() {
    socket.non_blocking(true);
    socket.native_non_blocking(true);

    socket.async_read_some(
            boost::asio::buffer(buffer, bufferSize),
            boost::bind(&Connection::handleRead, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)

    );
}

void Connection::handleRead(const boost::system::error_code &error, size_t size) {
    if (error) {
        stop();
        return;
    }
    request.parseRequest(std::string(buffer), size,
                         std::bind(&Connection::sendMessage, shared_from_this(), std::placeholders::_1),
                         std::bind(&Connection::sendFile, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Connection::handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred) {
    std::cerr << "handleWrite\n";
    if (error) {
        std::cerr << "handleWrite: error\n";
        stop();
        return;
    }
}

void Connection::sendMessage(const std::string &message) {
    std::cerr << "sendMessage\n";

    socket.async_write_some(
            boost::asio::buffer(message, message.size()),
            boost::bind(&Connection::handleWrite, shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred)

    );
}

void Connection::sendFile(int fd, size_t size) {
    std::cerr << "sendFile\n";
    ssize_t result = 0;
    while (offset < size) {
        result = sendfile(socket.native_handle(), fd, &offset, filePartSize);
        if (result < 0) {
            std::cerr << "error: " << errno << std::endl;
            if (errno == 32 || errno == 104) {
                stop();
                return;
            }
        }
    }
}