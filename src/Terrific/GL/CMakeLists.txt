#cmake_minimum_required(VERSION 3.12.2)
cmake_minimum_required(VERSION 3.5.1)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(TerrificGL VERSION 1 LANGUAGES CXX)

set(TerrificGL_SRCS
  SphereDrawable.cpp
  )
set(TerrificGL_HEADERS
  SphereDrawable.h
  )

add_library(TerrificGLObjects OBJECT
	${TerrificGL_SRCS}
	${TerrificGL_HEADERS})
target_include_directories(TerrificGLObjects PUBLIC 
	${JSON_INCLUDE_DIR}
	../../
	$<TARGET_PROPERTY:Terrific,INTERFACE_INCLUDE_DIRECTORIES>
	)

set_target_properties(TerrificGLObjects PROPERTIES FOLDER "Terrific/GL")
set_target_properties(TerrificGLObjects PROPERTIES COMPILE_FLAGS -fPIC)

add_library(TerrificGL ${SHARED_OR_STATIC}

	$<TARGET_OBJECTS:TerrificGLObjects>)

set_target_properties(TerrificGL PROPERTIES 
	DEBUG_POSTFIX "-d" 
	FOLDER "Terrific/GL")

target_link_libraries(TerrificGL Terrific)

install(TARGETS TerrificGL
	RUNTIME DESTINATION ${TERRIFIC_BINARY_INSTALL_DIR}
	LIBRARY DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR}
	ARCHIVE DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR})

install(FILES ${TerrificGL_HEADERS} DESTINATION ${TERRIFIC_INCLUDE_INSTALL_DIR}/GL)

add_library(Terrific::GL ALIAS TerrificGL)

if(BUILD_TESTS)
  #		add_subdirectory(Test)
endif()
