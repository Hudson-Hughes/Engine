cmake_minimum_required(VERSION 3.22)

project(game LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)

add_subdirectory( Engine )

add_subdirectory( Game )