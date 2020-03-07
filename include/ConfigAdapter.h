//
// Created by Ian on 15.02.2020.
//

#ifndef HIGHLOAD_CONFIGADAPTER_H
#define HIGHLOAD_CONFIGADAPTER_H

#include <fstream>
#include <any>

//	Provides an interface for
//	reading specific config file format
//	in "key-value" form
class ConfigAdapter{
public:
	enum STATUS {OK, eof, unopened};
protected:
	std::ifstream _stream;
public:
	ConfigAdapter();
	//	Open the specific config file
	//	Returns 0 if success and -1 if error case
	int Open(const std::string filename);

	//	Returns EOF flag if file was ended
	int Next();

	//	Reads pair[param_name, param_value]
	virtual std::pair<std::string, std::any> Get() = 0;
};

#endif //HIGHLOAD_CONFIGADAPTER_H
