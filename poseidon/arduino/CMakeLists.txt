cmake_minimum_required (VERSION 2.8.12)
set(PROJECT_NAME arduino)
project (${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")
set(SOURCE_DIR ${${PROJECT_NAME}_SOURCE_DIR})

include_directories(${SOURCE_DIR}/)
include_directories(${SOURCE_DIR}/../../utils/arduino/emulator/include)
include_directories(${SOURCE_DIR}/../../utils/arduino/Hub/include)
include_directories(${SOURCE_DIR}/../../utils/arduino/mock)
include_directories(${SOURCE_DIR}/../../utils/arduino/mock/redirects)

file(GLOB_RECURSE SOURCE_FILES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/../../utils/arduino/emulator/src/*.cpp" "${SOURCE_DIR}/../../utils/arduino/Hub/src/*.cpp" "${SOURCE_DIR}/../../utils/arduino/mock/redirects/__MOCK__SOURCE__.cpp")

# remove CMakeCXXCompilerId.cpp and test files
foreach(SOURCE_FILE ${SOURCE_FILES})
  if(SOURCE_FILE MATCHES "CMakeCXXCompilerId|spec.cpp|\\.piolibdeps")
    LIST(REMOVE_ITEM SOURCE_FILES ${SOURCE_FILE})
  endif()
endforeach()

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
