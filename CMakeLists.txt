cmake_minimum_required(VERSION 3.17)
project(VerleEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads)

add_executable(VerleEngine main.cpp world.cpp world.hpp vector2.hpp point.hpp)

target_include_directories(VerleEngine PUBLIC 3rdparty/wykobi)

target_link_libraries(VerleEngine Threads::Threads -lsfml-graphics -lsfml-window -lsfml-system GL)

