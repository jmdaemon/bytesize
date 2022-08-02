set(HEADERS ${INCLUDES_DIR} ${HEADERS_LOG_C} ${HEADERS_UNITY})
set(TARGET bytesize)

# Set up our first and only test suite
set(TEST_NAME test_${TARGET}-bin)

# Build the test code and runner
add_executable(${TEST_NAME} test_bytesize.c)
target_link_libraries(${TEST_NAME}
    ${TARGET}_static
    Unity)

# Add main test runner
add_test(test_${TARGET} ${TEST_NAME})
