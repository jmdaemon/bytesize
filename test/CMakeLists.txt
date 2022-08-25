set(HEADERS ${INCLUDES_DIR} ${HEADERS_VERSION} ${HEADERS_LOG_C} ${HEADERS_UNITY})

# Note that if you want to include more tests, create an executable and add them with add_test()

set(TARGET bytesize)
set(TEST_NAME test_${TARGET}-bin)
add_executable(${TEST_NAME} test_bytesize.c)
target_link_libraries(${TEST_NAME} ${TARGET}_static Unity)
target_include_directories(${TEST_NAME} PUBLIC ${HEADERS})
add_test(
    NAME ${TEST_NAME}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMAND ${TEST_NAME})
