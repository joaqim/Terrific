#cmake_minimum_required(VERSION 3.12.2)
cmake_minimum_required(VERSION 3.5.1)

# CMake policies. 0025 needs to be before project(), so putting all there.
# Use AppleClang instead of Clang on Apple
if(POLICY CMP0025)
	cmake_policy(SET CMP0025 NEW)
endif()
# Don't treat imported targets with :: as files
if(POLICY CMP0028)
	cmake_policy(SET CMP0028 NEW)
endif()
# Enable MACOSX_RPATH by default
if(POLICY CMP0042)
	cmake_policy(SET CMP0042 NEW)
endif()
# Quoted variables should not be dereferenced
if(POLICY CMP0054)
	cmake_policy(SET CMP0054 NEW)
endif()

if(POLICY CMP0048)
	cmake_policy(SET CMP0048 NEW)
endif()


# Configuration fails on < 3.4 if only C++ is enabled (CheckFunctionExists
# macro called from FindX11). Also, Android needs C for compiling some glue
# code.
if(NOT CMAKE_VERSION VERSION_LESS 3.4.0 AND NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
	set(LANG CXX)
endif()

project(Terrific)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake_modules)

option(BUILD_STATIC "Build static libraries (default are shared)" OFF)

option(BUILD_STATIC_PIC "Build static libraries and plugins with position-independent code" ON)
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
	find_package(Corrade REQUIRED TestSuite) 
	enable_testing() 
endif()

if(BUILD_STATIC) 
	set(TERRIFIC_BUILD_STATIC 1) 
endif()
if(NOT BUILD_STATIC) 
	set(SHARED_OR_STATIC SHARED) 
else() 
	set(SHARED_OR_STATIC STATIC) 
endif()

set(DEBUG_UI ON)
set(BUILD_UI ON)

if(NOT BUILD_STATIC)
	set(SHARED_OR_STATIC SHARED)
else()
	set(SHARED_OR_STATIC STATIC)
endif()

# SOL2

#include(${CMAKE_MODULE_PATH}/sol2.cmake)
add_subdirectory("third_party/repos/sol2")

include_directories("third_party/include")

# Installation paths

set(TERRIFIC_BINARY_INSTALL_DIR bin)
set(TERRIFIC_LIBRARY_INSTALL_DIR lib${LIB_SUFFIX})
set(TERRIFIC_DATA_INSTALL_DIR ${TERRIFIC_INCLUDE_INSTALL_PREFIX}/share/magnum)
set(TERRIFIC_CMAKE_MODULE_INSTALL_DIR ${TERRIFIC_INCLUDE_INSTALL_PREFIX}/share/cmake/Terrific)
set(TERRIFIC_INCLUDE_INSTALL_DIR ${TERRIFIC_INCLUDE_INSTALL_PREFIX}/include/Terrific)
set(TERRIFIC_EXTERNAL_INCLUDE_INSTALL_DIR ${TERRIFIC_INCLUDE_INSTALL_PREFIX}/include/TerrificExternal)
set(TERRIFIC_PLUGINS_INCLUDE_INSTALL_DIR ${TERRIFIC_INCLUDE_INSTALL_PREFIX}/include/TerrificPlugins)


# Library version
set(TERRIFIC_LIBRARY_VERSION 1.0)
set(TERRIFIC_LIBRARY_SOVERSION 1)

add_subdirectory(src)

if(BUILD_TESTS)
	#add_subdirectory
endif()
