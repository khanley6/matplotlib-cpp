cmake_minimum_required(VERSION 3.7)

##
## PROJECT
## name and version
##
project(matplotlibcpp VERSION 0.1.0 LANGUAGES CXX)

find_package(PythonLibs REQUIRED)
find_package(xtensor REQUIRED)

OPTION(USE_NUMPY "Use numpy" ON)
OPTION(BUILD_EXAMPLES "Build examples" OFF)

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
set(MPLCPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(matplotlibcpp INTERFACE)
target_include_directories(matplotlibcpp INTERFACE $<BUILD_INTERFACE:${MPLCPP_INCLUDE_DIR}>
                                                   $<INSTALL_INTERFACE:include>)

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()



# Installation
# ============

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS matplotlibcpp
        EXPORT ${PROJECT_NAME}-targets)

# Makes the project importable from the build directory
export(EXPORT ${PROJECT_NAME}-targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

   #install(DIRECTORY ${MPLCPP_INCLUDE_DIR}
   install(FILES ${MPLCPP_INCLUDE_DIR}/matplotlibcpp.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# GNUInstallDirs "DATADIR" wrong here; CMake search path wants "share".
set(MPLCPP_CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE STRING "install path for matplotlibcppConfig.cmake")

configure_package_config_file(${PROJECT_NAME}Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                              INSTALL_DESTINATION ${MPLCPP_CMAKECONFIG_INSTALL_DIR})

# matplotlibcpp is header-only and does not depend on the architecture.
# Remove CMAKE_SIZEOF_VOID_P from matplotlibcppConfigVersion.cmake so that
# an matplotlibcppConfig.cmake generated for a 64 bit target can be used for
# 32 bit targets and vice versa.
set(_MPLCPP_CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                                 VERSION ${${PROJECT_NAME}_VERSION}
                                 COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
              DESTINATION ${MPLCPP_CMAKECONFIG_INSTALL_DIR})
install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${MPLCPP_CMAKECONFIG_INSTALL_DIR})
