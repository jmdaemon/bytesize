set(HEADERS ${INCLUDES_DIR} ${HEADERS_VERSION} ${HEADERS_LOG_C})

set(TARGET bytesize)

# Bytesize bin
set(BIN_NAME ${TARGET}-bin)
add_executable(${BIN_NAME}
    # Main sources
    bytesize.c main.c cli.c
    # Subproject sources
    ## Log.c Sources
    "${PROJECT_SOURCE_DIR}/subprojects/log.c/src/log.c")
target_include_directories(${BIN_NAME} PUBLIC ${HEADERS})
target_link_libraries(${BIN_NAME} PRIVATE m pcre gmp mpfr)

# Rename binary target without -bin suffix
set_target_properties(${BIN_NAME} PROPERTIES OUTPUT_NAME ${TARGET})

# Bytesize lib

## Shared library
set(LIB_NAME ${TARGET})
add_library(${LIB_NAME}
    SHARED # Build as a dynamic library
    # Main sources
    bytesize.c
    # Subproject sources
    ## Log.c Sources
    "${PROJECT_SOURCE_DIR}/subprojects/log.c/src/log.c")
target_include_directories(${LIB_NAME} PUBLIC ${HEADERS})
target_link_libraries(${LIB_NAME} PRIVATE m pcre gmp mpfr)

## Static library
set(LIB_NAME ${TARGET}_static)
add_library(${LIB_NAME}
    STATIC # Build as a static library
    # Main sources
    bytesize.c
    # Subproject sources
    ## Log.c Sources
    "${PROJECT_SOURCE_DIR}/subprojects/log.c/src/log.c")
target_include_directories(${LIB_NAME} PUBLIC ${HEADERS})
target_link_libraries(${LIB_NAME} PRIVATE m pcre gmp mpfr)

# Manual Installation
install(TARGETS ${BIN_NAME} DESTINATION bin)    # /usr/local/bin/bytesize
install(TARGETS ${TARGET} DESTINATION lib)      # /usr/local/lib/libbytesize.so

#install(TARGETS ${OGGEX} DESTINATION bin)
#install(TARGETS ${OGGEX} CONFIGURATIONS Debug   RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin/Debug)
#install(TARGETS ${OGGEX} CONFIGURATIONS Release RUNTIME DESTINATION ${PROJECT_BINARY_DIR}/bin/Release)
