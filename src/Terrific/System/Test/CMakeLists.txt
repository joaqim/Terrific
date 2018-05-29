
set(SYSTEM_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake 
	${CMAKE_CURRENT_BINARY_DIR}/configure.h)

if(true)

add_executable(SystemTest main.cpp)

target_link_libraries(SystemTest Terrific TerrificSystem)
target_include_directories(SystemTest PUBLIC ${TERRIFIC_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}/third_party/include 
	${PROJECT_SOURCE_DIR}/third_party/repos/MetaStuff/include)

set_target_properties( SystemTest PROPERTIES FOLDER "Terrific/System/Test")

else()
	corrade_add_test(SystemTest main.cpp
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
