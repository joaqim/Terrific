cmake_minimum_required(VERSION 3.5.1)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(TerrificMeta VERSION 1 LANGUAGES CXX)

set(TerrificMeta_SRCS
	StringCast.cpp
  )
set(TerrificMeta_HEADERS
	JsonCast.h
	JsonCast.inl
	MovieInfo.h
	Person.h
	StringCast.h 
  )


add_library(TerrificMetaObjects OBJECT
	${TerrificMeta_SRCS}
	${TerrificMeta_HEADERS})
target_include_directories(TerrificMetaObjects PUBLIC
	${JSON_INCLUDE_DIR}
	../../
	$<TARGET_PROPERTY:Terrific,INTERFACE_INCLUDE_DIRECTORIES>
	)

set_target_properties(TerrificMetaObjects PROPERTIES FOLDER "Terrific/Meta")
set_target_properties(TerrificMetaObjects PROPERTIES COMPILE_FLAGS "-fPIC")

add_library(TerrificMeta ${SHARED_OR_STATIC}
	$<TARGET_OBJECTS:TerrificMetaObjects>)
set_target_properties(TerrificMeta PROPERTIES COMPILE_FLAGS "-fPIC")
set_target_properties(TerrificMeta PROPERTIES 
	DEBUG_POSTFIX "-d" 
	FOLDER "Terrific/Meta")

# EntityPlus
set(EntityPlus_DIR ${CMAKE_SOURCE_DIR}/third_party/repos/EntityPlus)
#add_library(EntityPlus INTERFACE)
#set_target_properties(EntityPlus PROPERTIES COMPILE_FLAGS "-w -Wunused-variable") # Suppress Warnings
#add_subdirectory(${EntityPlus_DIR})
#target_include_directories(EntityPlus INTERFACE ${EntityPlus_DIR})

include_directories(SYSTEM ${EntityPlus_DIR})

#target_link_libraries(TerrificMeta EntityPlus)

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
target_link_libraries(TerrificMeta sol2 ${LUA53_LIBRARIES})

# ecst
include_directories(${CMAKE_SOURCE_DIR}/third_party/repos/ecst/include)

target_link_libraries(TerrificMeta Terrific)

install(TARGETS TerrificMeta
	RUNTIME DESTINATION ${TERRIFIC_BINARY_INSTALL_DIR}
	LIBRARY DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR}
	ARCHIVE DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR})

install(FILES ${TerrificMeta_HEADERS} DESTINATION ${TERRIFIC_INCLUDE_INSTALL_DIR}/Meta)

add_library(Terrific::Meta ALIAS TerrificMeta)

if(BUILD_TESTS)
  	add_subdirectory(Test)
endif()


