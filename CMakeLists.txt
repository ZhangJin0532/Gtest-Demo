cmake_minimum_required(VERSION 3.5.0)
project(l1 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

include_directories("./inc")
add_subdirectory("./src/lib/leptjson")

aux_source_directory(./src PROJECT_SOURCE)

add_executable(l1 ${PROJECT_SOURCE})
target_link_libraries(l1 leptjson gtest pthread)
