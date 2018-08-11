cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

#Project name
project(folder_generator_by_date)

# To include all header files present in the include folder
include_directories(include)
include_directories(~/Downloads/boost_1_67_0/)
include_directories(~/Downloads/eigen3/)

# adding source files manualy
#set(SOURCES src/Folder.cpp src/Network.cpp)

# add source files recursively
file(GLOB SOURCES "src/*.cpp")

# for static library
set( PROJECT_LINK_LIBS -lstdc++fs )

add_executable( run_me ${SOURCES})

target_link_libraries(run_me ${PROJECT_LINK_LIBS} )
