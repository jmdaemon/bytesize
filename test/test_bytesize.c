#include "bytesize.h"
#include "unity.h"

// Unity Test Framework Configurations
#define UNITY_INCLUDE_DOUBLE

void setUp(void) {
    /* set stuff up here */
}

void tearDown(void) {
    /* clean stuff up here */
}

void calc_factor_si_should_return_correct_bytescale() {
    // The scale is with the respect of the unit to the scale of one byte measurement
    const char* unit = "KB";
    const unsigned int scale = 1000;
    const long int expected = 1000;
    const long int result = calc_factor(unit, SIZE, SI_BYTE, scale);
    // 1 KB == 1000 Bytes
    TEST_ASSERT_EQUAL_INT(expected, result);
}


void calc_factor_si_byte_should_return_correct_bytescale() {
    const char* unit = "B";
    const unsigned int scale = 1;
    const long int expected = 1;
    const long int result = calc_factor(unit, SIZE, SI_BYTE, scale);
    TEST_ASSERT_EQUAL_INT(expected, result);
}

void calc_factor_binary_should_return_correct_bytescale() {
}

void conversion_binary_to_si_should_return_correct_bytescale() {
    const char* from = "5MiB";
    const char* units_from = "MiB";
    const char* units_to = "MB";

    const double result = convert_units(from, units_from, units_to, false);
    const double expected = 5.242880;
    printf("Result  : %f\n", result);
    printf("Expected: %f\n", expected);
    TEST_ASSERT_EQUAL_FLOAT(result, expected);
}

/* Main runner */
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(calc_factor_si_should_return_correct_bytescale);
    RUN_TEST(calc_factor_si_byte_should_return_correct_bytescale);
    RUN_TEST(calc_factor_binary_should_return_correct_bytescale);
    RUN_TEST(conversion_binary_to_si_should_return_correct_bytescale);
    return UNITY_END();
}
