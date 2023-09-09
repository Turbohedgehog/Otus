cmake_minimum_required(VERSION 3.10)
#cmake_minimum_required(VERSION 3.16)


set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VESRION})
set (CMAKE_CXX_STANDARD 17)

#cmake_policy(SET CMP0074 OLD)
#cmake_policy(SET CMP0074 NEW)
#find_package(Boost 1.84.0 COMPONENTS program_options REQUIRED)
#find_package (Boost REQUIRED COMPONENTS program_options)

#set (Boost_DEBUG ON)
message("~~~~ ${PROJECT_SOURCE_DIR}")
#set (BOOST_ROOT C:/Projects/boost)
#smessage("~~~~ ${BOOST_ROOT}")

#set (BOOST_INCLUDE_DIR ${BOOST_ROOT})
#set (Boost_NO_SYSTEM_PATHS ON)
#set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
#set (Boost_USE_STATIC_RUNTIME OFF)
#set (BOOST_ALL_DYN_LINK OFF)
#message("~~~~ ${Boost_INCLUDE_DIRS}")
#message("~~~~ ${Boost_LIBRARIES}")
#find_package (Boost REQUIRED COMPONENTS python39 program_options)
find_package(Boost REQUIRED COMPONENTS program_options)
#message("~~~~ ${Boost_INCLUDE_DIRS}")
#message("~~~~ ${Boost_LIBRARIES}")
#find_package(Boost)
#set(Boost_USE_STATIC_LIBS ON)

file (
    GLOB
    PROJECT_SOURCE_FILES
    *.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(Boost_FOUND)
  #include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries (${PROJECT_NAME} Boost::boost Boost::program_options)
  #target_link_libraries (${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT unlinker@mail.ru)

include(CPack)