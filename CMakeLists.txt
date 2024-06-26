cmake_minimum_required(VERSION 3.25)
project(mstch)

option(WITH_UNIT_TESTS "enable building unit test executable" OFF)
option(WITH_BENCHMARK "enable building benchmark executable" OFF)
option(WITH_INSTALLER "enable installer" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(mstch_VERSION 1.0.4)

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -O3")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif()

add_subdirectory(src)

if(WITH_UNIT_TESTS)
  enable_testing()
  add_subdirectory(vendor/headerize)
  add_subdirectory(test)
endif()

if(WITH_BENCHMARK)
  add_subdirectory(vendor/benchmark)
  add_subdirectory(benchmark)
endif()
