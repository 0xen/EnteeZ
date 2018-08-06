cmake_minimum_required(VERSION 2.6)

set(project_name EnteeZ-demo)
project(${project_name})

set(src
    src/main.cpp
)

set(headers
    include/TestComponent.hpp
)

include_directories(include)
include_directories(../EnteeZ/include)


add_executable(${project_name} ${src} ${headers})

target_link_libraries(${project_name} EnteeZ)