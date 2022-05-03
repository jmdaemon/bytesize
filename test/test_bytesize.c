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
    RUN_TEST(calc_factor_binary_should_return_correct_bytescale);
    RUN_TEST(conversion_binary_to_si_should_return_correct_bytescale);
    return UNITY_END();
}
