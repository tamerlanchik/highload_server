cmake_minimum_required(VERSION 3.10)
project(HighLoad)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
find_package(Boost 1.58.0 REQUIRED system)

add_executable(HighLoad src/main.cpp src/ConfigAdapter.cpp src/ConfigParser.cpp src/Server.cpp src/HttpParser.cpp
        src/Server.cpp src/NetWorker.cpp include/Conntection.h src/Connection.cpp include/Request.h include/Response.h src/Request.cpp src/Response.cpp)

target_link_libraries(HighLoad -lboost_thread -lboost_filesystem ${Boost_LIBRARIES})