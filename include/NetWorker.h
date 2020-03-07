//
// Created by Ian on 16.02.2020.
//

#ifndef HIGHLOAD_NETWORKER_H
#define HIGHLOAD_NETWORKER_H

#include <uv.h>

class NetWorker {
public:
	static int Work(uv_stream_t* server, ...);
};

#endif //HIGHLOAD_NETWORKER_H
