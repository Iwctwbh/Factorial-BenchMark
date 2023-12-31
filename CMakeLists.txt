﻿# CMakeList.txt : CMake project for emsdk, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("emsdk")

# Add source to this project's executable.
add_executable (emsdk "Factorial_BenchMark.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET emsdk PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

#  gmp
find_package(PkgConfig REQUIRED)
pkg_check_modules(gmp REQUIRED IMPORTED_TARGET gmp)
target_link_libraries(emsdk PkgConfig::gmp)

# gmpxx
find_package(PkgConfig REQUIRED)
pkg_check_modules(gmpxx REQUIRED IMPORTED_TARGET gmpxx)
target_link_libraries(emsdk PkgConfig::gmpxx)

# fmt
find_package(fmt CONFIG REQUIRED)
target_link_libraries(emsdk fmt::fmt)