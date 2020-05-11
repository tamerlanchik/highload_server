//
// Created by andrey on 11.05.2020.
//
#include <iostream>
#include <vector>
#include <Request.h>

using std::string;
using std::vector;

Request::Request(std::string dir): rootDir(dir) {}

void Request::parseRequest(string request, size_t size,
        std::function<void (const string&)> sendHeader,
        std::function<void (int, size_t)> sendFile) {
    std::cerr << "parseRequest\n";
    std::istringstream iss(request);
    iss >> method;

    if (!validateMethod()) {
        response.notAllowed(sendHeader);
        return;
    }

    const size_t header_limit = 100;
    iss >> url >> version;
    iss.ignore(header_limit, '\n'); // \n after HTTP/1.1
    iss.ignore(header_limit, '\n'); // Host: localhost:8000
    iss.ignore(header_limit, ':');  // Accept-Encoding:
    iss >> encoding;
    iss.ignore(header_limit, '\n'); // \n after identity
    iss.ignore(header_limit, ':');  // Content-Length:
    iss >> contentLength;

    if (method == GET) {
        std::cerr << "get\n";
        response.get(rootDir, url, sendHeader, std::move(sendFile), true);
        return;
    } else if (method == HEAD) {
        std::cerr << "head\n";
        response.head(rootDir, url, sendHeader);
    }

}

bool Request::validateMethod() {
    return method == GET || method == HEAD;
}