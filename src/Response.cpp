#include "Response.h"
#include "boost/filesystem/operations.hpp"
#include "boost/progress.hpp"
#include "boost/algorithm/string.hpp"
#include <fstream>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

bool urlDecode(const std::string &in, std::string &out) {
    out.clear();
    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
        else if (in[i] == '+') {
            out += ' ';
        }
        else {
            out += in[i];
        }
    }
    return true;
}

string removeQuery(const std::string &path) {
    std::size_t found = path.find_last_of('?');
    string withoutQuery = path.substr(0, found);
    return withoutQuery;
}

const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%a, %d %B %Y %X GMT", &tstruct);

    return buf;
}

void Response::head(string rootDir, string originalPath, std::function<void (const string&)> sendHeader) {
    get(rootDir, originalPath, sendHeader, nullptr, false);
}

void Response::get(string rootDir, string originalPath, std::function<void (const string&)> sendHeader, std::function<void (int, size_t)> sendFile, bool isSendFile) {
    string path, pathWithoutQuery;
    pathWithoutQuery = removeQuery(originalPath);

    if (!urlDecode(pathWithoutQuery, path)) {
        badRequest(sendHeader); // wrong url
    }

    fs::path relativePath(rootDir + path);
    bool dirExist = false;
    if (is_directory(relativePath)) { // Directory index file name index.html
        relativePath /= "index.html";
        dirExist = true;
    }

    boost::system::error_code ec;
    fs::path absolutePath = canonical(relativePath, rootDir, ec);
    if (ec) {
        perror("");
        if (dirExist) {
            forbidden(sendHeader); // this dir exist but file index.html does not exist
        } else {
            notFound(sendHeader); // this file/dir does not exist
        }
        return;
    }

    if(!checkRootDir(absolutePath.string(), rootDir)) {
        notFound(sendHeader); // this file over the root_path
        return;
    }

    boost::uintmax_t filesize = file_size(absolutePath, ec);

    string contentType = typeDefinition(absolutePath.string());
    string length = headerContentLength + std::to_string(filesize);

    sendHeader(headerOk + mainHeaders() + contentType + length + "\r\n\r\n");

    if (isSendFile) {
        int fd = open(absolutePath.c_str(), O_RDONLY);
        sendFile(fd, filesize);
        close(fd);
    }
}

bool Response::checkRootDir(string rootDir, string path) {
    return boost::algorithm::contains(rootDir, path);
}

string Response::typeDefinition(string file) {
    std::size_t found = file.find_last_of('.');
    string type = file.substr(found + 1);
    return headerContentType + Response::_content_types.at(type) + "\r\n";
}

void Response::notFound(std::function<void (const string&)> sendHeader) {
    sendHeader(headerNotFound + mainHeaders());
}

void Response::notAllowed(std::function<void(const string &)> sendHeader) {
    sendHeader(headerNotAllowed + mainHeaders());
}

void Response::forbidden(std::function<void(const string &)> sendHeader) {
    sendHeader(headerForbidden + mainHeaders());
}

void Response::badRequest(std::function<void(const string &)> sendHeader) {
    sendHeader(headerBadRequest + mainHeaders());
}

string Response::mainHeaders() {
    string currentDate = headerDate + currentDateTime() + "\r\n"; // Wed, 03 May 2020 07:28:00 GMT
    return currentDate + headerServer + headerConnection;
}

std::map<std::string, std::string> Response::_content_types = {
        {"html", "text/html"},
        {"txt", "text/txt"},
        {"css", "text/css"},
        {"gif", "image/gif"},
        {"jpeg","image/jpeg"},
        {"jpg", "image/jpeg"},
        {"png", "image/png"},
        {"js", "application/javascript"},
        {"swf", "application/x-shockwave-flash"},
};