cmake_minimum_required(VERSION 3.5.1)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)


set(GEOMETRY_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake 
	${CMAKE_CURRENT_BINARY_DIR}/configure.h)

find_package(Corrade REQUIRED TestSuite)
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

enable_testing()

#file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/safe_reference.lua
#DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

corrade_add_test(SphericalVoronoiTest
	SphericalVoronoiTest.cpp
  LIBRARIES TerrificGeometry TerrificUtility TerrificMath
	)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${TERRIFIC_INCLUDE_DIR}
	)


# List of Tests
#set_target_properties(
#GeometryTest
#PROPERTIES FOLDER "Terrific/Geometry/Test")
#set_target_properties(GeometryTest PROPERTIES COMPILE_FLAGS "-w -Wunused-variable -Wmissing-prototypes") # Suppress Warnings

