cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../googletest [EXCLUDE_FROM_ALL])
set(BUILD_GMOCK OFF)


set(PROJECT_CLI ${PROJECT_NAME}_cli)
set(PROJECT_TEST test_${PROJECT_NAME})

add_executable(${PROJECT_CLI} mail.cpp)
add_library(${PROJECT_NAME} ip_filter.cpp)
add_executable(${PROJECT_TEST} ip_filter_test.cpp)

set_target_properties(${PROJECT_CLI} ${PROJECT_NAME} ${PROJECT_TEST} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(${PROJECT_CLI} PRIVATE
${PROJECT_NAME}
)

target_link_libraries(${PROJECT_TEST}
    GTest::gtest_main
    ${PROJECT_NAME}
)


if (MSVC)
    target_compile_options(${PROJECT_CLI} PRIVATE
        /W4
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
    )
    target_compile_options(${PROJECT_TEST} PRIVATE
        /W4
    )
else ()
    target_compile_options(${PROJECT_CLI} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(${PROJECT_TEST} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS ${PROJECT_CLI} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT unlinker@mail.ru)

include(CPack)

enable_testing()

add_test(${PROJECT_TEST} test_ip_filter)
