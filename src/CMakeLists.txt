# Include header files
set(INCLUDES_DIR "${PROJECT_SOURCE_DIR}/include")
set(HEADERS_LOG_C "${PROJECT_SOURCE_DIR}/subprojects/log.c/src")
set(HEADERS_UNITY "${PROJECT_SOURCE_DIR}/subprojects/unity/src")
set(HEADERS ${INCLUDES_DIR} ${HEADERS_LOG_C} ${HEADERS_UNITY})

# Bytesize bin
set(BIN_NAME bytesize)

## Copy our subprojects to the destination
#add_custom_command(${BIN_NAME} ${CMAKE_PROJECT_NAME} PRE_BUILD
                   #COMMAND ${CMAKE_COMMAND} -E create_symlink
                   #${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:${PROJECT_NAME}>/config)

#add_executable(bytesize bytesize.c main.c cli.c)
add_executable(${BIN_NAME}
    # Main sources
    bytesize.c main.c cli.c
    # Log.c Sources
    "${PROJECT_SOURCE_DIR}/subprojects/log.c/src/log.c"
    )
    # Unity sources
    #"${PROJECT_SOURCE_DIR}/subprojects/unity/src/unity.c")
target_include_directories(${BIN_NAME} PUBLIC ${HEADERS})
target_link_libraries(${BIN_NAME} PRIVATE m pcre gmp mpfr)

# Bytesize lib
