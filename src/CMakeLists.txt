set(HEADERS ${INCLUDES_DIR} ${HEADERS_VERSION} ${HEADERS_LOG_C})

set(TARGET bytesize)

# Bytesize bin
set(BIN_NAME ${TARGET}-bin)
add_executable(${BIN_NAME} bytesize.c main.c cli.c)
target_include_directories(${BIN_NAME} PUBLIC ${HEADERS})
target_link_libraries(${BIN_NAME} PRIVATE ${PKG_DEPENDS})

# Rename binary executable without -bin suffix
set_target_properties(${BIN_NAME} PROPERTIES OUTPUT_NAME ${TARGET})

# Bytesize lib

## Shared library
set(LIB_NAME ${TARGET})
add_library(${LIB_NAME}
    SHARED # Build as a dynamic library
    bytesize.c)
target_include_directories(${LIB_NAME} PUBLIC ${HEADERS})
set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER "${INCLUDES_DIR}/bytesize.h")
target_link_libraries(${LIB_NAME} PRIVATE ${PKG_DEPENDS})

## Static library
set(LIB_NAME ${TARGET}_static)
add_library(${LIB_NAME}
    STATIC # Build as a static library
    bytesize.c)
target_include_directories(${LIB_NAME} PUBLIC ${HEADERS})
target_link_libraries(${LIB_NAME} PRIVATE ${PKG_DEPENDS})

# Manual Installation
# Binary
install(TARGETS ${BIN_NAME}
    DESTINATION bin
    COMPONENT bin)        # /usr/local/bin/bytesize

# Library
install(TARGETS
    ${TARGET} DESTINATION lib
    PUBLIC_HEADER DESTINATION include   # Install headers
    COMPONENT lib)        # /usr/local/lib/libbytesize.so
