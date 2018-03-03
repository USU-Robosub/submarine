cmake_minimum_required (VERSION 2.8.12)
set(PROJECT_NAME system)
project (${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")
set(SOURCE_DIR ${${PROJECT_NAME}_SOURCE_DIR})

include_directories(${SOURCE_DIR}/)
include_directories(${SOURCE_DIR}/../../extern)

set(tests OFF CACHE BOOL "" FORCE)
set(coverage OFF CACHE BOOL "" FORCE)
add_subdirectory(${SOURCE_DIR}/../../utils/cpp ${CMAKE_CURRENT_BINARY_DIR}/utils/cpp)
include_directories(${Utils_Includes})

add_definitions(-DENABLE_OPEN_CV)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Utils)
