cmake_minimum_required(VERSION 2.6)

set(project_name EnteeZ)
project(${project_name})

set(common_headers
    include/EnteeZ/EnteeZ.hpp
    include/EnteeZ/Entity.hpp
)

set(common_source
    src/EnteeZ.cpp
    src/Entity.cpp
)

include_directories(include)

set(source
    ${common_source}
)

set(headers
    ${common_headers}
)

add_library(${project_name} STATIC ${source} ${headers})