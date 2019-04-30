cmake_minimum_required(VERSION 3.7)

##
## PROJECT
## name and version
##
project(matplotlibcpp_xt VERSION 0.1.0 LANGUAGES CXX)

find_package(PythonLibs REQUIRED)
find_package(xtensor REQUIRED)

##
## TARGET
## create target and add include path
##
add_library(matplotlibcpp INTERFACE)

add_subdirectory(examples)
