//
// Created by Ian on 15.02.2020.
//
#include <ConfigParser.h>
#include <ConfigAdapter.h>
#include <iostream>

ConfigParser::ConfigParser(std::shared_ptr<Config> ptr) {
	_config = ptr;
}

int ConfigParser::parse(ConfigAdapter& src) {
	while(ConfigAdapter::eof != src.Next()) {
	    std::cerr << "Conf parse iteration start\n";
		auto val = src.Get();
		_config->Put(val.first, val.second);
	}
	return 0;
}

bool Config::Put(std::string name, std::any val) {
	_map[name] = val;
	return true;
}

std::any Config::Get(const std::string name) {
//	_map["cpu_limit"] = 5;
	return _map[name];
}

std::string Config::GetString(const std::string name) {
	return std::any_cast<std::string>(Get(name));
}
int Config::GetInt(const std::string name) {
	std::any val = Get(name);
	return std::any_cast<int>(val);
}
