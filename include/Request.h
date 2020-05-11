//
// Created by andrey on 11.05.2020.
//

#ifndef HIGHLOAD_REQUEST_H
#define HIGHLOAD_REQUEST_H

#include <cstdlib>
#include <boost/asio.hpp>
#include "Response.h"

using std::string;

class Request {
public:
    Request(string);

    void parseRequest(string, size_t, std::function<void (const string&)>, std::function<void (int, size_t)>);

    bool validateMethod();
private:
    string method;
    string url;
    string version;
    string encoding;
    string contentLength;

    const string GET = "GET";
    const string HEAD = "HEAD";

    string rootDir;
    Response response;
};

#endif //HIGHLOAD_REQUEST_H
