cmake_minimum_required(VERSION 3.5)

set(project_name EnteeZ-tests)
project(${project_name})

set(src
    src/main.cpp
)

include_directories(../EnteeZ/include)

add_executable(${project_name} ${src})

target_link_libraries(${project_name} EnteeZ)

add_test(NAME ${project_name} COMMAND ${project_name})
