cmake_minimum_required (VERSION 2.8.12)
set(PROJECT_NAME poseidon_arduino)
project (${PROJECT_NAME})

option(coverage "Enable coverage output" OFF)

# ===============================================
# =                 C++ Flags
# ===============================================
add_definitions(-DUNIT_TESTS)
set(CMAKE_CXX_CUSTOM_FLAGS "-std=gnu++11 -pthread")
if(${coverage})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_CUSTOM_FLAGS} -coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -coverage")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_CUSTOM_FLAGS}")
endif()
set(SOURCE_DIR ${${PROJECT_NAME}_SOURCE_DIR})

# ===============================================
# =           Include Utils/Arduino
# ===============================================
set(tests OFF CACHE BOOL "" FORCE)
set(coverage OFF CACHE BOOL "" FORCE)
add_subdirectory(${SOURCE_DIR}/../../utils/arduino ${CMAKE_CURRENT_BINARY_DIR}/utils/arduino)
include_directories(${utils_arduino_Includes})

# ===============================================
# =               Source Files
# ===============================================
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "main.cpp")
foreach(SOURCE_FILE ${SOURCE_FILES}) # remove CMakeCXXCompilerId.cpp and test files
  if(SOURCE_FILE MATCHES "CMakeCXXCompilerId|spec.cpp")
    LIST(REMOVE_ITEM SOURCE_FILES ${SOURCE_FILE})
  endif()
endforeach()

# ===============================================
# = force no standard libraries in arduino code
# ===============================================
foreach(SOURCE_FILE ${SOURCE_FILES})
  message(" - ${SOURCE_FILE}")
  set_source_files_properties(${SOURCE_FILE} PROPERTIES COMPILE_FLAGS "-nostdlib -fno-exceptions -fno-rtti -nostdinc++ -nostdinc")
endforeach()

# ===============================================
# =            Include Directories
# ===============================================
include_directories(${SOURCE_DIR}/include)
include_directories(${SOURCE_DIR}/../../extern)
include_directories(${SOURCE_DIR}/mock)
include_directories(${utils_arduino_Mocks})

# ===============================================
# =               Unit Test App
# ===============================================
file(GLOB_RECURSE SPEC_FILES "*.spec.cpp")
add_executable(${PROJECT_NAME}_tests
  "${SOURCE_DIR}/../../extern/catch2/main.cpp"
  ${SPEC_FILES}
  ${SOURCE_FILES}
  ${utils_arduino_MockSrc}
)
target_link_libraries(${PROJECT_NAME}_tests utils_arduino)
