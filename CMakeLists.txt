cmake_minimum_required(VERSION 3.7)

##
## PROJECT
## name and version
##
project(matplotlibcpp_xt VERSION 0.1.0 LANGUAGES CXX)

find_package(PythonLibs REQUIRED)
find_package(xtensor REQUIRED)

OPTION(USE_NUMPY "Use numpy" ON)

if (USE_NUMPY)
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
    find_package(Numpy REQUIRED)
    message(STATUS "Found numpy: ${NUMPY_INCLUDE_DIRS}")
else()
    add_compile_definitions(WITHOUT_NUMPY)
endif()


##
## TARGET
## create target and add include path
##
add_library(matplotlibcpp INTERFACE)

add_subdirectory(examples)
