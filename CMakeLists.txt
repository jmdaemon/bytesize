cmake_minimum_required(VERSION 3.12)

project(
    bytesize
    VERSION 1.10.0
    DESCRIPTION "Convert between Binary & SI Bytes"
    LANGUAGES C)

if (NOT USE_AS_SUBPROJECT)
    # Variable Toggles
    set(NO_COLOR OFF CACHE INTERNAL "Toggles colored output errors")
    set(BUILD_DOC ON CACHE INTERNAL "Build documentation")
    set(ENABLE_LOGGING OFF CACHE INTERNAL "View the logging statements/debug info")
    set(LOG_LEVEL 0 CACHE INTERNAL "Enable only specific log levels")
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

if (NO_COLOR)
    # Don't use colored output
else()
    # Include colored output by default
    include(Color)
endif()

# Include header files
set(INCLUDES_DIR "${PROJECT_SOURCE_DIR}/include")
set(HEADERS_VERSION "${PROJECT_BINARY_DIR}/include")

# Single source project versions
configure_file(
  "${PROJECT_SOURCE_DIR}/include/version.h.in"
  "${PROJECT_BINARY_DIR}/include/version.h")

# Include dependencies
include(FetchContent)

add_subdirectory(subprojects)

set(USE_AS_SUBPROJECT TRUE)
set(USE_AS_SUBMODULE TRUE)

include(utility)
include(logc)

unset(USE_AS_SUBPROJECT)
unset(USE_AS_SUBMODULE)

# Specify project dependencies
# PKG_DEPENDS: Library names to link the output files 
# DEB_DEPENDS: Debian packages that provide the libraries the project depends upon
set(PKG_DEPENDS m pcre gmp mpfr logc utility)
set(DEB_DEPENDS libc libpcre2-posix2 libpcre2-dev libgmp-dev libmpfr-dev)

# Add subdirectories
add_subdirectory(src)

if(BUILD_TESTING AND NOT USE_AS_SUBPROJECT)
    include(Unity)
    include(CTest)
    add_subdirectory(test)
endif()

# Generate automatic docs with Doxygen
include(Docs)

# Install build artifacts system-wide
include(Install)

# Packages
include(InstallRequiredSystemLibraries)
include(Debian)

# Packaging
cpack_add_component(bin
    DISPLAY_NAME "Application"
    DESCRIPTION ${PROJECT_DESCRIPTION})

cpack_add_component(lib
    DISPLAY_NAME "Library"
    DESCRIPTION "Shared library with header used for ${PROJECT_NAME}")
