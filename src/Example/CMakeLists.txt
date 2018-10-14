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
  )

  # Corrade Resources
  #corrade_add_resource(MyApplication_RESOURCES ${PROJECT_SOURCE_DIR}/data/resources.conf)
  #list(APPEND SOURCES ${MyApplication_RESOURCES}) 

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

target_link_libraries(MyApplication PRIVATE ${LIBRARIES})

