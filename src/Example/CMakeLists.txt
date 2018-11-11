find_package(Magnum REQUIRED
  GL
  Shaders
  Trade
  SceneGraph
  MeshTools
  Primitives
  AnyImageImporter
  TgaImporter
  )

find_package(MagnumPlugins REQUIRED
  PngImporter)

list(APPEND LIBRARIES
  Magnum::Application
  Magnum::GL
  Magnum::Magnum
  Magnum::Shaders
  Magnum::Trade
  Magnum::SceneGraph
  Magnum::MeshTools
  Magnum::Primitives
  Magnum::AnyImageImporter
  Magnum::TgaImporter
  MagnumPlugins::PngImporter

  Terrific::Geometry
  Terrific::Math
  Terrific::Utility
  Terrific::GL
  )

if(NOT CORRADE_TARGET_ANDROID)
  find_package(Magnum REQUIRED Sdl2Application)
else()
  find_package(Magnum REQUIRED AndroidApplication)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

#configure_file(${PROJECT_SOURCE_DIR}/src/configure.h.cmake ${PROJECT_SOURCE_DIR}/src/configure.h)
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

list(APPEND SOURCES
  main.cpp
  Application.cpp
  #SphereDrawable.cpp
  Bitmap.cpp
  Planet.cpp
  )

# Corrade Resources
corrade_add_resource(MyApplication_RESOURCES ${CMAKE_SOURCE_DIR}/data/resources.conf)
list(APPEND SOURCES ${MyApplication_RESOURCES}) 

if(NOT CORRADE_TARGET_ANDROID)
  add_executable(MyApplication ${SOURCES})
else()
  add_library(MyApplication SHARED ${SOURCES})
endif()

set_target_properties(MyApplication PROPERTIES COMPILE_FLAGS "-w -Wunused-parameter") # Suppress Warnings

# Corrade
find_package(Corrade REQUIRED
  Interconnect
  )
list(APPEND LIBRARIES
  Corrade::Interconnect
  )

# FastNoiseSIMD
set(FastNoise_DIR ${CMAKE_SOURCE_DIR}/third_party/repos/FastNoiseSIMD)
file(GLOB FastNoise_SRCS ${FastNoise_DIR}/FastNoiseSIMD/*.cpp)

#list(APPEND FastNoise_SRCS ${FastNoise_DIR}/FastNoiseSIMD/FastNoiseSIMD.cpp)
#list(APPEND FastNoise_SRCS ${FastNoise_DIR}/FastNoiseSIMD/FastNoiseSIMD_internal.cpp)

#list(APPEND FastNoise_SRCS ${FastNoise_DIR}/FastNoiseSIMD/FastNoiseSIMD_avx512.cpp)
#list(APPEND FastNoise_SRCS ${FastNoise_DIR}/FastNoiseSIMD/FastNoiseSIMD_avx512.cpp)


set_source_files_properties(FastNoiseSIMD_sse2.cpp PROPERTIES COMPILE_FLAGSE "-msse2")
set_source_files_properties(FastNoiseSIMD_sse41.cpp PROPERTIES COMPILE_FLAGSE "-msse4.1")
set_source_files_properties(FastNoiseSIMD_avx2.cpp PROPERTIES COMPILE_FLAGSE "-mavx2 -march=core-avx2")
set_source_files_properties(FastNoiseSIMD_avx512.cpp PROPERTIES COMPILE_FLAGSE "-mavx512f")

add_library(FastNoise ${FastNoise_SRCS})

set_target_properties(FastNoise PROPERTIES COMPILE_FLAGS "-w -march=core-avx2") # Suppress Warnings

include_directories(SYSTEM ${FastNoise_DIR}/FastNoiseSIMD)


list(APPEND LIBRARIES FastNoise)


target_link_libraries(MyApplication PRIVATE ${LIBRARIES})

