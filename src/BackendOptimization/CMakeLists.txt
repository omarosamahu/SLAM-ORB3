# CMakeList.txt : CMake project for NonLinear_optimization, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("NonLinear_optimization")

find_package(OpenCV REQUIRED)
# Add source to this project's executable.
add_executable (NonLinear_optimization NonLinear_optimization.cpp)

add_subdirectory(Thirdparty/g2o)

target_link_libraries(NonLinear_optimization 
                          PUBLIC
                          ORB_SLAM::g2o
                          ${OpenCV_LIBRARIES})

target_include_directories(NonLinear_optimization 
                           PUBLIC 
                           ${OpenCV_INCLUDE_DIRS}
                           ${CMAKE_CURRENT_SOURCE_DIR}/include)


# TODO: Add tests and install targets if needed.
