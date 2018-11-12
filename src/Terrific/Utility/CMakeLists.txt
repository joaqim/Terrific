cmake_minimum_required(VERSION 3.5.1)

project(TerrificUtility VERSION 1 LANGUAGES CXX)
#set(CMAKE_CXX_STANDARD 17)
add_library(TerrificUtility Log.cpp)

find_package(spdlog CONFIG REQUIRED)
find_package(Threads REQUIRED)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
	find_library(log-lib log)
	target_link_libraries(TerrificUtility spdlog::spdlog Threads::Threads log)
else()
	target_link_libraries(TerrificUtility spdlog::spdlog Threads::Threads)
endif()

target_link_libraries(TerrificUtility spdlog::spdlog Threads::Threads)

 install(TARGETS TerrificUtility
         RUNTIME DESTINATION ${TERRIFIC_BINARY_INSTALL_DIR}
         LIBRARY DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR}
         ARCHIVE DESTINATION ${TERRIFIC_LIBRARY_INSTALL_DIR})

 install(FILES ${TerrificUtility_HEADERS} DESTINATION
	    ${TERRIFIC_INCLUDE_INSTALL_DIR}/Utility)
      
      add_library(Terrific::Utility ALIAS TerrificUtility)
#add_executable(example example.cpp)
#target_link_libraries(example TerrificUtility)

#TODO: Temporary for debugging
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/build/logs")
