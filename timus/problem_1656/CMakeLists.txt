cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(task VERSION ${PROJECT_VESRION})


set(PROJECT_CLI ${PROJECT_NAME}_cli)

add_executable(${PROJECT_CLI} main.cpp)

set_target_properties(${PROJECT_CLI} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${PROJECT_CLI} PRIVATE)
