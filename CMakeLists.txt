cmake_minimum_required(VERSION 2.6)

set(project_name demo)
project(${project_name})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(EnteeZ)
add_subdirectory(EnteeZ-demo)
