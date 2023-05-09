cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

#[[
if (MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()


add_subdirectory(googletest)
set(BUILD_GMOCK OFF)
]]

#configure_file(version.h.in version.h)

set(PROJECT_CLI ${PROJECT_NAME}_cli)

add_executable(${PROJECT_CLI} mail.cpp)
add_library(${PROJECT_NAME} ip_filter.cpp)
#add_executable(test_${PROJECT_NAME} test_version.cpp)

#set_target_properties(${PROJECT_CLI} ${PROJECT_NAME} test_version PROPERTIES
set_target_properties(${PROJECT_CLI} ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(${PROJECT_CLI} PRIVATE
${PROJECT_NAME}
)

#[[
target_link_libraries(test_version
    GTest::gtest_main
    helloworld
)
]]

#[[
if (MSVC)
    target_compile_options(helloworld_cli PRIVATE
        /W4
    )
    target_compile_options(helloworld PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(helloworld_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
]]
install(TARGETS ${PROJECT_CLI} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT unlinker@mail.ru)

include(CPack)

#[[
enable_testing()

add_test(test_version test_version)
]]