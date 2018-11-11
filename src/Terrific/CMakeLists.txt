cmake_minimum_required(VERSION 3.5.1)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake 
	${CMAKE_CURRENT_BINARY_DIR}/configure.h)


set(Terrific_SRCS Impl/StringCast.cpp)
set(Terrific_HEADERS 
	Impl/JsonCast.h
	Impl/JsonCast.inl
	Impl/MovieInfo.h
	Impl/Person.h
	Impl/StringCast.h
	)

add_library(TerrificObjects OBJECT
	${Terrific_SRCS}
	${Terrific_HEADERS})
target_include_directories(TerrificObjects PUBLIC
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_BINARY_DIR}/src)
if(NOT BUILD_STATIC) 
	target_compile_definitions(TerrificObjects PRIVATE "TerrificObjects_EXPORTS") 
endif() 
if(NOT BUILD_STATIC OR BUILD_STATIC_PIC) 
	set_target_properties(TerrificObjects PROPERTIES POSITION_INDEPENDENT_CODE ON) 
endif()
set_target_properties(TerrificObjects PROPERTIES FOLDER "Terrific")

# Main Library
add_library(Terrific ${SHARED_OR_STATIC} 
	$<TARGET_OBJECTS:TerrificObjects>
  )

set_target_properties(Terrific PROPERTIES 
	DEBUG_POSTFIX "-d" 
	FOLDER "Terrific")

install(TARGETS Terrific
	RUNTIME DESTINATION ${TERRIFIC_BINARY_INSTALL_DIR}
	LIBRARY DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR}
	ARCHIVE DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR})

install(FILES ${Terrific_HEADERS} DESTINATION ${TERRIFIC_INCLUDE_INSTALL_DIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/configure.h DESTINATION ${TERRIFIC_INCLUDE_INSTALL_DIR})


if(WITH_UTILITY)
add_subdirectory(Utility)
endif()
if(WITH_MATH)
add_subdirectory(Math)
endif()
if(WITH_SYSTEM)
add_subdirectory(System)
endif()
if(WITH_GEOMETRY)
add_subdirectory(Geometry)
endif()
if(WITH_GL)
add_subdirectory(GL)
endif()

if(BUILD_TESTS) 
	#  add_subdirectory(Test) 
endif()

add_library(Terrific::Terrific ALIAS Terrific)
set(_TERRIFIC_CONFIGURE_FILE ${CMAKE_CURRENT_BINARY_DIR}/configure.h CACHE INTERNAL "")
