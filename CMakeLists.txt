cmake_minimum_required(VERSION 3.12)

project(g2vpp C CXX)

option(G2V_USE_CVFFMPEG "Use OpenCV+ffmpeg encoder" ON)
option(G2V_USE_NVENC "Use NVenc encoder" OFF)

list(APPEND G2V_SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/src/g2v.cpp)

if(G2V_USE_CVFFMPEG)
    list(APPEND G2V_SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/src/g2v_cvffmpeg.cpp)
    message("Using OpenCV+ffmpeg encoder")
endif()

if(G2V_USE_NVENC)
    list(APPEND G2V_SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/src/g2v_nvenc.cpp)
    message("Using NVenc encoder")
endif()

add_library(g2vpp ${G2V_SOURCE_FILES})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(g2vpp PRIVATE glfw)

if(NOT TARGET glad)
    add_library(g2v_glad ${CMAKE_CURRENT_LIST_DIR}/glad/glad.c)
    target_link_libraries(g2vpp PRIVATE g2v_glad)
else()
    target_link_libraries(g2vpp PRIVATE glad)
endif()
target_include_directories(g2vpp PRIVATE glad)

if(G2V_USE_CVFFMPEG)
    find_package(OpenCV REQUIRED)
    target_include_directories(g2vpp PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(g2vpp PUBLIC ${OpenCV_LIBS} opencv_videoio)
    message(${OpenCV_LIBS})
endif()

if(G2V_USE_NVENC)

endif()

target_include_directories(g2vpp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(g2vpp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/glad)

set(G2V_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)
