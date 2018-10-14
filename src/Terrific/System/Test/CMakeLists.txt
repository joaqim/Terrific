cmake_minimum_required(VERSION 3.12.2)

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)


set(SYSTEM_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake 
	${CMAKE_CURRENT_BINARY_DIR}/configure.h)

if(false)

  add_executable(SystemTest SerializationTest.cpp)

  target_link_libraries(SystemTest Terrific TerrificSystem)
  target_include_directories(SystemTest PUBLIC ${TERRIFIC_INCLUDE_DIR}
	  ${PROJECT_SOURCE_DIR}/third_party/include
	  ${PROJECT_SOURCE_DIR}/third_party/repos/MetaStuff/include)

  set_target_properties( SystemTest PROPERTIES FOLDER "Terrific/System/Test")

else()
  find_package(Corrade REQUIRED TestSuite)
  set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

  enable_testing()

  corrade_add_test(SystemTest SerializationTest.cpp
	  LIBRARIES Terrific TerrificSystem
	  )

  target_include_directories(
	  SystemTest
	  PRIVATE 
	  ${CMAKE_CURRENT_BINARY_DIR}
	  ${TERRIFIC_INCLUDE_DIR}
	  )


  # List of Tests
  set_target_properties(
	  SystemTest
	  PROPERTIES FOLDER "Terrific/System/Test")

endif()
