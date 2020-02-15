//
// Created by Ian on 15.02.2020.
//

#include <ConfigAdapter.h>

ConfigAdapter::ConfigAdapter() {}

int ConfigAdapter::Open(const std::string filename) {
	_stream = std::ifstream(filename);
	if(_stream.is_open()) {
		return OK;
	} else {
		perror("");
		return unopened;
	}
}

int ConfigAdapter::Next() {
	return static_cast<int>(_stream.eof());
}
