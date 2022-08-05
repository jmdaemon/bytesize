set(HEADERS ${INCLUDES_DIR} ${HEADERS_VERSION} ${HEADERS_LOG_C} ${HEADERS_UNITY})

# Note that if you want to include more tests, create an executable and add them with add_test()

# Set up our first and only test suite
set(TARGET bytesize)
set(TEST_NAME test_${TARGET}-bin)

# Build the test code and runner
add_executable(${TEST_NAME} test_bytesize.c)
target_link_libraries(${TEST_NAME}
    ${TARGET}_static
    Unity)
target_include_directories(${TEST_NAME} PUBLIC ${HEADERS})

# Add main test runner
add_test(test_${TARGET} ${TEST_NAME})
