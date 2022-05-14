# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("ipmt")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Include sub-projects.
add_subdirectory ("src")
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
