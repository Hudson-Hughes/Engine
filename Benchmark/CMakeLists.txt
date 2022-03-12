cmake_minimum_required(VERSION 3.10)
project(mytest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

#set(BLA_VENDOR Intel10_64lp)

#find_package(BLAS REQUIRED)

add_executable(test main.cpp)
#target_link_libraries(test ${BLAS_LIBRARIES})