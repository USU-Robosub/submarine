cmake_minimum_required (VERSION 2.8.12)
set(PROJECT_NAME utils_arduino)
project (${PROJECT_NAME})

message("building arduino utils =================================")

option(tests "Build tests" OFF)
option(coverage "Enable coverage output" OFF)

set(CMAKE_CXX_CUSTOM_FLAGS "-std=gnu++11")

if(${coverage})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_CUSTOM_FLAGS} -coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -coverage")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_CUSTOM_FLAGS}")
endif()
set(SOURCE_DIR ${${PROJECT_NAME}_SOURCE_DIR})

set(Includes ${SOURCE_DIR}/include ${SOURCE_DIR}/Hub/include)
set(Mocks "${SOURCE_DIR}/mock" "${SOURCE_DIR}/Hub/mock")
set(${PROJECT_NAME}_Includes ${Includes} PARENT_SCOPE)
set(${PROJECT_NAME}_Mocks ${Mocks} PARENT_SCOPE)

include_directories(${Includes})

file(GLOB_RECURSE SOURCE_FILES "Hub/src/*.cpp")

# remove CMakeCXXCompilerId.cpp and test files
foreach(SOURCE_FILE ${SOURCE_FILES})
  if(SOURCE_FILE MATCHES "CMakeCXXCompilerId|spec.cpp")
    LIST(REMOVE_ITEM SOURCE_FILES ${SOURCE_FILE})
  endif()
endforeach()

add_definitions(-DUNIT_TESTS)

include_directories(${Mocks})
add_library(${PROJECT_NAME} ${SOURCE_FILES})

file(GLOB_RECURSE MOCK_SPEC_FILES "*.mock.spec.cpp")
get_filename_component(GLOBAL_MOCK "test/global.cpp" REALPATH)
list(APPEND MOCK_SPEC_FILES ${GLOBAL_MOCK})
set(${PROJECT_NAME}_Mock_Spec ${MOCK_SPEC_FILES} PARENT_SCOPE)

if(${tests})
  file(GLOB_RECURSE SPEC_FILES "*.spec.cpp")
  include_directories(${SOURCE_DIR}/../../extern)

  add_executable(${PROJECT_NAME}_Tests test/main.cpp test/global.cpp ${SPEC_FILES})
  target_link_libraries(${PROJECT_NAME}_Tests ${PROJECT_NAME})
endif()
