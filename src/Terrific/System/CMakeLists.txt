cmake_minimum_required(VERSION 3.12.2)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(TerrificSystem VERSION 1 LANGUAGES CXX)

set(TerrificSystem_SRCS SerializeClass.cpp)
set(TerrificSystem_HEADERS SerializeClass.h)

add_library(TerrificSystemObjects OBJECT
	${TerrificSystem_SRCS}
	${TerrificSystem_HEADERS})
target_include_directories(TerrificSystemObjects PUBLIC 
	${JSON_INCLUDE_DIR}
	../../
	$<TARGET_PROPERTY:Terrific,INTERFACE_INCLUDE_DIRECTORIES>
	)

set_target_properties(TerrificSystemObjects PROPERTIES FOLDER "Terrific/System")

add_library(TerrificSystem ${SHARED_OR_STATIC}

	$<TARGET_OBJECTS:TerrificSystemObjects>)

set_target_properties(TerrificSystem PROPERTIES 
	DEBUG_POSTFIX "-d" 
	FOLDER "Terrific/System")

target_link_libraries(TerrificSystem Terrific)

install(TARGETS TerrificSystem
	RUNTIME DESTINATION ${TERRIFIC_BINARY_INSTALL_DIR}
	LIBRARY DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR}
	ARCHIVE DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR})

install(FILES ${TerrificSystem_HEADERS} DESTINATION ${TERRIFIC_INCLUDE_INSTALL_DIR}/System)

add_library(Terrific::System ALIAS TerrificSystem)

if(BUILD_TESTS)
		add_subdirectory(Test)
endif()
