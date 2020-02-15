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
	//	Add value of any type
	bool Put(std::string name, std::any);

	//	Get parameter value of any type
	std::any Get(const std::string name);

	//	Get string parameter value
	std::string GetString(const std::string name);

	//	Get integer parameter value
	int GetInt(const std::string name);
};

class ConfigParser {
private:
	std::shared_ptr<Config> _config;
public:
	//	Takes a shared_ptr to the Config to inflate
	ConfigParser(std::shared_ptr<Config> ptr);

	//	Do not depend from config file format
	//	Takes an adapter for specific config format
	//	Returns 0 on success
	int parse(ConfigAdapter& src);
};

#endif //HIGHLOAD_CONFIGPARSER_H
