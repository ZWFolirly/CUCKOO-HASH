cmake_minimum_required(VERSION 3.10)
project(concurrency)

set(CMAKE_CXX_STANDARD 14)

add_executable(concurrency main.cpp hash.h test.h)

include_directories(/media/wangluobin/Story/研究生/路由器原理与设计/libcuckoo-master/libcuckoo)
find_package(Threads REQUIRED)
target_link_libraries(concurrency Threads::Threads)