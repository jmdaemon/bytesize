cmake_minimum_required(VERSION 3.12)

project(
    bytesize
    VERSION 0.2.0
    DESCRIPTION "Convert between Binary & SI Bytes"
    LANGUAGES C)

# Use colored output
if (NOT COLOR)
    # Don't use colored output
else()
    # Include colored output by default
    include(Color)
endif()

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with "
                        "CMakeLists.txt file). Please make a build subdirectory. Feel free to "
                        "remove CMakeCache.txt and CMakeFiles.")
endif()

# Include CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/modules/" "${PROJECT_SOURCE_DIR}/cmake")

# Include header files
set(INCLUDES_DIR "${PROJECT_SOURCE_DIR}/include")
set(HEADERS_LOG_C "${PROJECT_SOURCE_DIR}/subprojects/log.c/src")
if(BUILD_TESTING)
    message(STATUS "Setting \$\{HEADERS_UNITY\}")
    set(HEADERS_UNITY "${PROJECT_SOURCE_DIR}/subprojects/unity/src")
endif()

# Single source project versions
#configure_file (
  #"${PROJECT_SOURCE_DIR}/include/Version.h.in"
  #"${PROJECT_BINARY_DIR}/include/Version.h"
#)

# Include dependencies
include(FetchContent)
include(Unity)

# Add subdirectories
add_subdirectory(src)

if(BUILD_TESTING)
    include(CTest)
    add_subdirectory(test)
endif()

# Package for debian
#include(Debian)

#set(CMAKE_INSTALL_PREFIX /usr/local)
#install(TARGETS ${OGGEX} DESTINATION bin)
#install(TARGETS ${OGGEX} CONFIGURATIONS Debug   RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug)
#install(TARGETS ${OGGEX} CONFIGURATIONS Release RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin/Release)
