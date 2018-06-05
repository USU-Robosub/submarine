cmake_minimum_required (VERSION 2.8.12)
set(PROJECT_NAME utils_arduino)
project (${PROJECT_NAME})

option(tests "Build tests" OFF)
option(coverage "Enable coverage output" OFF)

message("================================================")
message("The unit tests produced do not guarantee proper")
message("   intergration with the embeded hardware.")
message("================================================")

# ===============================================
# =                   Paths
# ===============================================
set(SOURCE_DIR ${${PROJECT_NAME}_SOURCE_DIR})
set(Includes # Locations to check for header files
  "${SOURCE_DIR}/include"
  "${SOURCE_DIR}/Hub/include"
)
set(Mocks # locations to check for mock types
  "${SOURCE_DIR}/mock"
  "${SOURCE_DIR}/Hub/mock"
  "${SOURCE_DIR}/mock/redirects"
)
set(Src # Source directories for arduino libraries
  "${SOURCE_DIR}/Hub/src/*.cpp"
)
set(MockSrc # Source directories for arduino libraries
  "${SOURCE_DIR}/mock/redirects/__MOCK__SOURCE__.cpp"
)

# ===============================================
# =                 C++ Flags
# ===============================================
add_definitions(-DUNIT_TESTS) # enable arduino mock files
set(CMAKE_CXX_CUSTOM_FLAGS "-std=gnu++11")
if(${coverage})
  message("=== Enabling Coverage for ${PROJECT_NAME}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_CUSTOM_FLAGS} -coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -coverage")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -coverage")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_CUSTOM_FLAGS}")
endif()

# ===============================================
# =               Source Files
# ===============================================
file(GLOB_RECURSE SOURCE_FILES ${Src})
foreach(SOURCE_FILE ${SOURCE_FILES}) # remove CMakeCXXCompilerId.cpp and test files
  if(SOURCE_FILE MATCHES "CMakeCXXCompilerId|spec.cpp")
    LIST(REMOVE_ITEM SOURCE_FILES ${SOURCE_FILE})
  endif()
endforeach()

# ===============================================
# = force no standard libraries in arduino code
# ===============================================
message("C/C++ standard libraries have been disabled for the following files (and any headers included by them):")
foreach(SOURCE_FILE ${SOURCE_FILES})
  message(" - ${SOURCE_FILE}")
  set_source_files_properties(${SOURCE_FILE} PROPERTIES COMPILE_FLAGS "-nostdlib -fno-exceptions -fno-rtti -nostdinc++ -nostdinc")
endforeach()

# ===============================================
# =            Include Directories
# ===============================================
include_directories(${Includes})
include_directories(${Mocks})

# ===============================================
# =              Static Library
# ===============================================
add_library(${PROJECT_NAME} ${SOURCE_FILES})

# ===============================================
# =               Unit Test App
# ===============================================
if(${tests})
  message("=== Building Unit Test App: ${PROJECT_NAME}_tests")
  file(GLOB_RECURSE SPEC_FILES "*.spec.cpp")
  include_directories(${SOURCE_DIR}/../../extern)

  add_library(${PROJECT_NAME}_mocks ${MockSrc})

  add_executable(${PROJECT_NAME}_tests
    "${SOURCE_DIR}/../../extern/catch2/main.cpp"
    ${SPEC_FILES}
  )
  target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME} ${PROJECT_NAME}_mocks)
endif()

# ===============================================
# =               Set parent vars
# ===============================================
get_directory_property(HAS_PARENT PARENT_DIRECTORY)
if(HAS_PARENT)
  set(${PROJECT_NAME}_Includes ${Includes} PARENT_SCOPE)
  set(${PROJECT_NAME}_Mocks ${Mocks} PARENT_SCOPE)
  set(${PROJECT_NAME}_MockSrc ${MockSrc} PARENT_SCOPE)
endif()
