cmake_minimum_required(VERSION 3.5.1)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)

project(TerrificMeta VERSION 1 LANGUAGES CXX)

set(TerrificMeta_SRC
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

target_link_libraries(TerrificMeta Terrific)

install(TARGETS TerrificMeta
	RUNTIME DESTINATION ${TERRIFIC_BINARY_INSTALL_DIR}
	LIBRARY DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR}
	ARCHIVE DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR})

install(FILES ${TerrificMeta_HEADERS} DESTINATION ${TERRIFIC_INCLUDE_INSTALL_DIR}/Meta)

add_library(Terrific::Meta ALIAS TerrificMeta)

if(BUILD_TESTS)
  #	add_subdirectory(Test)
endif()


