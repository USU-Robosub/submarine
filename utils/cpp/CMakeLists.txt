cmake_minimum_required (VERSION 2.8.12)
set(PROJECT_NAME Utils)
project (${PROJECT_NAME})

find_package( OpenCV )

option(tests "Build tests" OFF)
option(coverage "Enable coverage output" OFF)

set(CMAKE_CXX_CUSTOM_FLAGS "-std=gnu++11 -pthread")

if(${coverage})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_CUSTOM_FLAGS} -coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -coverage")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_CUSTOM_FLAGS}")
endif()
set(SOURCE_DIR ${${PROJECT_NAME}_SOURCE_DIR})

include_directories(${SOURCE_DIR}/include)
include_directories(${SOURCE_DIR}/../../extern/gsi)
include_directories(${SOURCE_DIR}/../../extern/rxcpp)
include_directories(${SOURCE_DIR}/../../extern/optional)

set(${PROJECT_NAME}_Includes ${SOURCE_DIR}/include PARENT_SCOPE)
set(${PROJECT_NAME}_Mocks ${SOURCE_DIR}/mock PARENT_SCOPE)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# remove CMakeCXXCompilerId.cpp and test files
foreach(SOURCE_FILE ${SOURCE_FILES})
  if(SOURCE_FILE MATCHES "CMakeCXXCompilerId|spec.cpp")
    LIST(REMOVE_ITEM SOURCE_FILES ${SOURCE_FILE})
  endif()
endforeach()

add_library(${PROJECT_NAME} ${SOURCE_FILES})

message("======= OpenCV =======")
if(${OpenCV_FOUND})
  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
  add_definitions(-DENABLE_OPEN_CV)
  message(" - Found OpenCV")
else()
  message(" - Missing OpenCV, not going to build related code.")
endif()

if(${tests})
  include_directories(${SOURCE_DIR}/mock)

  file(GLOB_RECURSE SPEC_FILES "*.spec.cpp")

  include_directories(${SOURCE_DIR}/../../extern/catch2)

  add_executable(${PROJECT_NAME}_Tests test/main.cpp ${SPEC_FILES})
  target_link_libraries(${PROJECT_NAME}_Tests ${PROJECT_NAME})
endif()
