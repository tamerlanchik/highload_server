//
// Created by Ian on 15.02.2020.
//

#ifndef HIGHLOAD_CONFIGADAPTER_H
#define HIGHLOAD_CONFIGADAPTER_H

#include <fstream>
#include <any>

class ConfigAdapter{
public:
	enum STATUS {OK, eof, unopened};
protected:
	std::ifstream _stream;
public:
	ConfigAdapter();
	int Open(const std::string filename);
	int Next();
	virtual std::pair<std::string, std::any> Get() = 0;
};

#endif //HIGHLOAD_CONFIGADAPTER_H
