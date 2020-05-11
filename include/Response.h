//
// Created by andrey on 11.05.2020.
//

#ifndef HIGHLOAD_RESPONSE_H
#define HIGHLOAD_RESPONSE_H

#include <boost/asio.hpp>
#include <string>
using std::string;

class Response {
private:
    bool checkRootDir(string, string);

    string typeDefinition(string);

    string mainHeaders();

public:
    static std::map<std::string, std::string> _content_types;

public:
    const string headerOk = string("HTTP/1.1 200 OK\r\n");
    const string headerBadRequest = string("HTTP/1.1 400 Bad Request\r\n");
    const string headerForbidden = string("HTTP/1.1 403 Forbidden\r\n");
    const string headerNotFound = string("HTTP/1.1 404 Not Found\r\n");
    const string headerNotAllowed = string("HTTP/1.1 405 Method Not Allowed\r\n");

    const string headerDate = string("Date: ");
    const string headerServer = string("Server: Seva/0.1 (Unix)\r\n");
    const string headerConnection = string("Connection: close\r\n");
    const string headerContentLength = string("Content-Length: ");
    const string headerContentType = string("Content-Type: ");

    void get(string, string, std::function<void (const string&)>, std::function<void (int, size_t)>, bool);
    void head(string, string, std::function<void (const string&)>);

    void notFound(std::function<void (const string&)>);
    void notAllowed(std::function<void (const string&)>);
    void forbidden(std::function<void (const string&)>);
    void badRequest(std::function<void (const string&)>);
};

#endif //HIGHLOAD_RESPONSE_H
