# Log.c
# Shared library
set(TARGET logc)
set(LIB_NAME logc)
add_library(${LIB_NAME} SHARED
    "${PROJECT_SOURCE_DIR}/subprojects/log.c/src/log.c")
target_include_directories(${LIB_NAME} PUBLIC ${HEADERS_LOG_C})
set_target_properties(${LIB_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS_LOG_C}/log.h")

# Static library
set(LIB_NAME ${TARGET}_static)
add_library(${LIB_NAME} STATIC
    "${PROJECT_SOURCE_DIR}/subprojects/log.c/src/log.c")
target_include_directories(${LIB_NAME} PUBLIC "${HEADERS_LOG_C}/log.h")
