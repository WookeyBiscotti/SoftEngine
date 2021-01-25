cmake_minimum_required(VERSION 3.17)
project(SoftEn)

set(CMAKE_CXX_STANDARD 20)

add_library(${CMAKE_PROJECT_NAME} src/world.cpp src/group/group_proxy.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 3rdparty/wykobi)

add_subdirectory(3rdparty/AABBTree)

target_link_libraries(${CMAKE_PROJECT_NAME} indexer)

set(BUILD_TEST_APP OFF CACHE BOOL "Build tests")

if(BUILD_TEST_APP)
	find_package(Threads)
	add_executable(${CMAKE_PROJECT_NAME}_TEST_APP main.cpp)
	target_link_libraries(${CMAKE_PROJECT_NAME}_TEST_APP SoftEn  Threads::Threads -lsfml-graphics -lsfml-window -lsfml-system GL)
endif()
