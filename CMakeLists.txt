cmake_minimum_required(VERSION 3.14)
project(Compilet LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)

include_directories(src)
add_executable(dominator src/dominator.cc)
