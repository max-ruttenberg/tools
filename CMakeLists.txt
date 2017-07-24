cmake_minimum_required(VERSION 2.6)
project(tools)

enable_testing()

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(tests)

set(TOOLS_LINK_DIR ${PROJECT_BINARY_DIR}/src PARENT_SCOPE)
set(TOOLS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include PARENT_SCOPE)
