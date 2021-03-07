# CMakeList.txt : CMake project for NonLinear_optimization, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("SLAM")

add_subdirectory(src/BackendOptimization)
add_subdirectory(src/FeatureExtraction)


# TODO: Add tests and install targets if needed.
