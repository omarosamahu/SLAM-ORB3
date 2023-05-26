# CMakeList.txt : CMake project for NonLinear_optimization, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("Thread Pool")
set(CXX_STANDARD_REQUIRED 14)
add_executable(ThreadPool main.cpp ThreadPool.cpp)
target_link_libraries(ThreadPool -pthread)