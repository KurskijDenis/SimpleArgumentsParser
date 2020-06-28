cmake_minimum_required(VERSION 3.10)

project(SimpleArgsParser)

set(MASTER_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MASTER_PROJECT ON)
endif ()

add_library(SimpleArgsParser Sources/ArgsParser.cpp Sources/ArgOptions.cpp Sources/ArgInfos.cpp Sources/ArgsParserException.cpp)

target_compile_options(SimpleArgsParser PRIVATE -std=c++17 -Wextra -Werror -Wall)
target_include_directories(SimpleArgsParser INTERFACE Headers)

if (${MASTER_PROJECT})
    enable_testing()
endif()
add_subdirectory(Tests)
