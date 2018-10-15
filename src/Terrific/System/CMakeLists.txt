#cmake_minimum_required(VERSION 3.12.2)
cmake_minimum_required(VERSION 3.5.1)

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

add_library(Terrific::System ALIAS TerrificSystem)

# EntityPlus
#set(EntityPlus_DIR ${CMAKE_SOURCE_DIR}/third_party/repos/EntityPlus)
#add_library(EntityPlus INTERFACE)
#set_target_properties(EntityPlus PROPERTIES COMPILE_FLAGS "-w -Wunused-variable") # Suppress Warnings
#add_subdirectory(${EntityPlus_DIR})
#target_include_directories(EntityPlus INTERFACE ${EntityPlus_DIR})

include_directories(SYSTEM ${EntityPlus_DIR})

#target_link_libraries(TerrificSystem EntityPlus)

# Sol2
find_package(Lua53 REQUIRED)
set(Sol2_DIR ${CMAKE_SOURCE_DIR}/third_party/repos/sol2 ${CMAKE_BINARY_DIR}/sol2)
#add_library(sol2 INTERFACE)
#include_directories( ${LUA_INCLUDE_DIR})
#set_target_properties(Sol2 PROPERTIES COMPILE_FLAGS "-w -Wunused-variable") # Suppress Warnings

#target_include_directories(Sol2 INTERFACE ${Sol2_DIR})

#include_directories(SYSTEM ${Sol2_DIR}/single/sol)
#add_subdirectory(${Sol2_DIR})
target_include_directories(sol2 INTERFACE ${LUA53_INCLUDE_DIR})
#target_link_libraries(sol2 ${LUA53_LIBRARIES})
target_link_libraries(TerrificSystem sol2 ${LUA53_LIBRARIES})

install(TARGETS TerrificSystem
	RUNTIME DESTINATION ${TERRIFIC_BINARY_INSTALL_DIR}
	LIBRARY DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR}
	ARCHIVE DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR})

install(FILES ${TerrificSystem_HEADERS} DESTINATION ${TERRIFIC_INCLUDE_INSTALL_DIR}/System)


if(BUILD_TESTS)
		add_subdirectory(Test)
endif()
