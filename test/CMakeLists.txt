cmake_minimum_required(VERSION 3.12)

project(g2vpp_test CXX)

add_executable(test main.cpp)

set(TARGET_NAME test)
include(${CMAKE_CURRENT_LIST_DIR}/../CMakeLists.txt)
target_link_libraries(test PRIVATE g2vpp)

# Use g2vpp embedded glad library
# To use modern (4.0+) GL functions, you should create your own glad library before include g2v directory

# Example: Put this line before "include_directories(${CMAKE_CURRENT_LIST_DIR}/..)" line
# add_library(glad ${CMAKE_CURRENT_LIST_DIR}/../glad/glad.c)

# Note: g2vpp still uses its own glad as an include directory,
# so you don't need to specify your glad include directory 
# for g2vpp, but your glad OpenGL version must be 3.0+
target_include_directories(test PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../glad)
target_link_libraries(test PRIVATE g2v_glad)
