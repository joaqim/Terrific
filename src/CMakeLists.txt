cmake_minimum_required(VERSION 3.5.1)
if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(OpenGL_GL_PREFERENCE "LEGACY")

set_directory_properties(PROPERTIES 
  CORRADE_CXX_STANDARD 14
  CORRADE_USE_PEDANTIC_FLAGS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake_modules/")

add_subdirectory(Terrific)

# Terrific include dir for superprojects
set(TERRIFIC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")


#if(!BUILD_TESTS)
#add_subdirectory(Example)
#endif()
add_subdirectory(TestPlanet)
