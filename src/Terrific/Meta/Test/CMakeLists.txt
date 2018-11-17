cmake_minimum_required(VERSION 3.5.1)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)


set(META_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake 
	${CMAKE_CURRENT_BINARY_DIR}/configure.h)

if(false)

  add_executable(MetaTest MetaTest.cpp)

  target_link_libraries(MetaTest Terrific TerrificMeta)
  target_include_directories(MetaTest SYSTEM ${TERRIFIC_INCLUDE_DIR}
	  ${PROJECT_SOURCE_DIR}/third_party/include
	  ${PROJECT_SOURCE_DIR}/third_party/repos/sol2
	  ${PROJECT_SOURCE_DIR}/third_party/repos/MetaStuff/include)

  set_target_properties( MetaTest PROPERTIES FOLDER "Terrific/Meta/Test")

else()
  find_package(Corrade REQUIRED TestSuite)
  set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

  enable_testing()

  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/safe_reference.lua
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  corrade_add_test(MetaTest
	  #SafeReference.cpp
    #    SerializationTest.cpp
    #CerealTest.cpp
    EntityTest.cpp
	  LIBRARIES Terrific TerrificMeta# ${LUA_LIBRARIES} sol2
	  )

target_include_directories(
	MetaTest
	PRIVATE 
	${CMAKE_CURRENT_BINARY_DIR}
	${TERRIFIC_INCLUDE_DIR}
	)


# List of Tests
set_target_properties(
	MetaTest
	PROPERTIES FOLDER "Terrific/Meta/Test")
#set_target_properties(MetaTest PROPERTIES COMPILE_FLAGS "-w -Wunused-variable -Wmissing-prototypes") # Suppress Warnings

endif()
