cmake_minimum_required (VERSION 2.8.12)
set(PROJECT_NAME Utils)
project (${PROJECT_NAME})

option(tests "Build tests" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")
set(SOURCE_DIR ${${PROJECT_NAME}_SOURCE_DIR})

include_directories(${SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# remove CMakeCXXCompilerId.cpp and test files
foreach(SOURCE_FILE ${SOURCE_FILES})
  if(SOURCE_FILE MATCHES "CMakeCXXCompilerId|spec.cpp")
    LIST(REMOVE_ITEM SOURCE_FILES ${SOURCE_FILE})
  endif()
endforeach()

add_library(${PROJECT_NAME} ${SOURCE_FILES})

if(${tests})
  include_directories(${SOURCE_DIR}/mocks)

  file(GLOB_RECURSE TEST_FILES "test/*.cpp")

  include_directories(../../extern/catch2)

  add_executable(${PROJECT_NAME}Tests ${TEST_FILES})
  target_link_libraries(${PROJECT_NAME}Tests ${PROJECT_NAME})
endif()
