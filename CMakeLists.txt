cmake_minimum_required(VERSION 2.6)

project(SPTE CXX)

# Useful CMake modules/macros are in a subdirectory
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CXX_FLAGS)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CXX_FLAGS)

enable_language(CXX)
include(CMakeDetermineCXXCompiler)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In source build is not supported, please 'git clean` and build out of source")
endif()

#Add the common library
add_subdirectory(SPTE_lib)
include_directories(${CMAKE_SOURCE_DIR}/SPTE_lib)

#If we have MPI, build the MPI reference
find_package(MPI)
if(MPI_CXX_FOUND)
	add_subdirectory(MPI_Reference)
endif()

