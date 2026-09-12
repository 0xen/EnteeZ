cmake_minimum_required(VERSION 3.5)

set(project_name demo)
project(${project_name})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(EnteeZ)
add_subdirectory(EnteeZ-demo)
