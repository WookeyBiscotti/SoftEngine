cmake_minimum_required(VERSION 3.17)
project(SoftEn)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads)

add_executable(${CMAKE_PROJECT_NAME} main.cpp world.cpp group/group_proxy.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 3rdparty/wykobi)
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC 3rdparty/RTree)

add_subdirectory(3rdparty/AABBTree)

target_link_libraries(${CMAKE_PROJECT_NAME} Threads::Threads indexer -lsfml-graphics -lsfml-window -lsfml-system GL)

