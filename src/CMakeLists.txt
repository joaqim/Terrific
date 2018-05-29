set_directory_properties(PROPERTIES 
	CORRADE_CXX_STANDARD 14
       	CORRADE_USE_PEDANTIC_FLAGS ON)

add_subdirectory(Terrific)

# Terrific include dir for superprojects
set(TERRIFIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")
