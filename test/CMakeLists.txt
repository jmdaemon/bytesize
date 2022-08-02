# Build ThrowTheSwitch's unity cmake project
set(UNITY_NAME Unity)
add_library(
    ${UNITY_NAME}
    STATIC
    "${PROJECT_SOURCE_DIR}/subprojects/unity/src/unity.c")

target_include_directories(${UNITY_NAME} PUBLIC ${HEADERS_UNITY})

# Build the test code and runner
set(HEADERS ${INCLUDES_DIR} ${HEADERS_LOG_C} ${HEADERS_UNITY})
set(TARGET bytesize)

# Set up our first and only test suite
set(TEST_NAME test_${TARGET}-bin)
add_executable(${TEST_NAME} test_bytesize.c)

target_link_libraries(${TEST_NAME}
    ${TARGET}_static
    Unity)


#add_executable(${TEST_NAME}
    #test_bytesize.c
    ##${TARGET}_static
    #"${PROJECT_BINARY_DIR}/src/lib${TARGET}_static.a"
    #Unity)

# Add main test runner
add_test(test_${TARGET} ${TEST_NAME})
