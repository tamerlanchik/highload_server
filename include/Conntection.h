//
// Created by andrey on 11.05.2020.
//

#ifndef HIGHLOAD_CONNTECTION_H
#define HIGHLOAD_CONNTECTION_H

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <sys/sendfile.h>
#include "Request.h"

using boost::asio::ip::tcp;

class Connection: public boost::enable_shared_from_this<Connection> {
public:
    typedef boost::shared_ptr<Connection> pointer;

    tcp::socket& getSocket() {
        return socket;
    };

    void start();

    Connection(boost::asio::io_service &, std::string);

    ~Connection();

private:

    void handleRead(const boost::system::error_code &, size_t);

    void handleWrite(const boost::system::error_code &, size_t);

    void sendMessage(const std::string &);

    void sendFile(int, size_t);

    void stop();

    tcp::socket socket;

    Request request;

    static const size_t bufferSize = 1024;
    char buffer[bufferSize];

    off_t offset;
    static const size_t filePartSize = 16384; // 2^14 16 kb

};

#endif //HIGHLOAD_CONNTECTION_H
