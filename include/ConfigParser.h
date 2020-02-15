//
// Created by Ian on 15.02.2020.
//

#ifndef HIGHLOAD_CONFIGPARSER_H
#define HIGHLOAD_CONFIGPARSER_H

#include <memory>
#include <ConfigAdapter.h>
#include <any>
#include <map>

class Config {
private:
	std::map<std::string, std::any> _map;
public:
	bool Put(std::string name, std::any);

	std::any Get(const std::string name);
	std::string GetString(const std::string name);
	int GetInt(const std::string name);
};

class ConfigParser {
private:
	std::shared_ptr<Config> _config;
public:
	ConfigParser(std::shared_ptr<Config> ptr);
	int parse(ConfigAdapter& src);
};

#endif //HIGHLOAD_CONFIGPARSER_H
