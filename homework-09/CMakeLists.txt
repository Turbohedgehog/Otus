cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(async VERSION ${PROJECT_VESRION})

set(PROJECT_NAME_CLI ${PROJECT_NAME}_cli)

file (
    GLOB
    PROJECT_SOURCE_FILES
    *.cpp
)

add_executable(${PROJECT_NAME_CLI} ${PROJECT_SOURCE_FILES})

set_target_properties(${PROJECT_NAME_CLI} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

add_library(${PROJECT_NAME} bulk.cpp async.cpp)

if (MSVC)
    target_compile_options(${PROJECT_NAME_CLI} PRIVATE
        /W4
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
    )
else ()
    target_compile_options(${PROJECT_NAME_CLI} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT unlinker@mail.ru)

include(CPack)