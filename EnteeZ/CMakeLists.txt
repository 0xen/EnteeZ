cmake_minimum_required(VERSION 2.6)

set(project_name EnteeZ)
project(${project_name})

set(common_headers
    include/EnteeZ/EnteeZ.hpp
    include/EnteeZ/Entity.hpp
    include/EnteeZ/BaseComponentWrapper.hpp
    include/EnteeZ/ComponentWrapper.hpp
    include/EnteeZ/EntityManager.hpp
    include/EnteeZ/TemplateBase.hpp
    include/EnteeZ/TemplateStorage.hpp
    include/EnteeZ/TemplatePair.hpp
)

set(common_source
    src/EnteeZ.cpp
    src/Entity.cpp
    src/BaseComponentWrapper.cpp
    src/ComponentWrapper.cpp
    src/EntityManager.cpp
    src/TemplateBase.cpp
    src/TemplateStorage.cpp
    src/TemplatePair.cpp
)

include_directories(include)

set(source
    ${common_source}
)

set(headers
    ${common_headers}
)

add_library(${project_name} STATIC ${source} ${headers})