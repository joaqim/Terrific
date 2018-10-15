cmake_minimum_required(VERSION 3.5.1)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(TerrificGeometry VERSION 1 LANGUAGES CXX)

set(TerrificGeometry_SRCS
  SphericalVoronoi.cpp
  )
set(TerrificGeometry_HEADERS
  SphericalVoronoi.h
  )


add_library(TerrificGeometryObjects OBJECT
	${TerrificGeometry_SRCS}
	${TerrificGeometry_HEADERS})
target_include_directories(TerrificGeometryObjects PUBLIC 
	${JSON_INCLUDE_DIR}
	../../
	$<TARGET_PROPERTY:Terrific,INTERFACE_INCLUDE_DIRECTORIES>
	)

set_target_properties(TerrificGeometryObjects PROPERTIES FOLDER "Terrific/Geometry")
set_target_properties(TerrificGeometryObjects PROPERTIES COMPILE_FLAGS -fPIC)
add_library(TerrificGeometry ${SHARED_OR_STATIC}

	$<TARGET_OBJECTS:TerrificGeometryObjects>)

set_target_properties(TerrificGeometry PROPERTIES 
	DEBUG_POSTFIX "-d" 
	FOLDER "Terrific/Geometry")

target_link_libraries(TerrificGeometry Terrific)

install(TARGETS TerrificGeometry
	RUNTIME DESTINATION ${TERRIFIC_BINARY_INSTALL_DIR}
	LIBRARY DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR}
	ARCHIVE DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR})

install(FILES ${TerrificGeometry_HEADERS} DESTINATION ${TERRIFIC_INCLUDE_INSTALL_DIR}/Geometry)

add_library(Terrific::Geometry ALIAS TerrificGeometry)

if(BUILD_TESTS)
  #	add_subdirectory(Test)
endif()




