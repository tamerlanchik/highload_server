FROM ubuntu:18.04
RUN apt-get update && apt-get install -y --no-install-recommends apt-utils
RUN apt-get -y install g++ cmake
RUN apt-get -y install libboost-all-dev
RUN apt-get install cpulimit

ADD ./ /container

WORKDIR /container



# RUN g++ src/*.cpp -lboost_thread -lboost_filesystem -std=c++14 -lboost_system -o httpd
# RUN apt-get -y install cmake protobuf-compiler
# RUN ls include/ -a
RUN cmake --version
RUN cmake ./ && cmake --build ./
# RUN make -o httpd
RUN ls -a

EXPOSE 8001

CMD ./HighLoad