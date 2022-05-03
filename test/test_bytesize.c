#include "bytesize.h"
#include "unity.h"

void setUp(void) {
    /* set stuff up here */
}

void tearDown(void) {
    /* clean stuff up here */
}

void calc_factor_si_should_return_correct_bytescale() {
    // The scale is with the respect of the unit to the scale of one byte measurement
    const char* unit = "KB";
    const long int expected = 1000;
    const long int result = calc_factor(unit, SIZE, SI_BYTE, SI_SCALE);
    // 1 KB == 1000 Bytes
    TEST_ASSERT_EQUAL_INT(expected, result);
}


void calc_factor_si_byte_should_return_correct_bytescale() {
    const char* unit = "B";
    const long int expected = 1;
    const long int result = calc_factor(unit, SIZE, SI_BYTE, SI_SCALE);
    // 1 Byte == 1 Byte
    TEST_ASSERT_EQUAL_INT(expected, result);
}

void calc_factor_binary_should_return_correct_bytescale() {
    // The scale is with the respect of the unit to the scale of one byte measurement
    const char* unit = "KiB";
    const long int expected = 1024;
    const long int result = calc_factor(unit, SIZE, BYTE, BINARY_SCALE);
    // 1 KiB == 1024 Bytes
    TEST_ASSERT_EQUAL_INT(expected, result);
}

void calc_factor_binary_byte_should_return_correct_bytescale() {
    const char* unit = "B";
    const long int expected = 1;
    const long int result = calc_factor(unit, SIZE, BYTE, BINARY_SCALE);
    // 1 B == 1 B
    TEST_ASSERT_EQUAL_INT(expected, result);
}

void convert_units_binary_to_si_should_return_correct_conversion() {
    const char* from = "5MiB";
    const char* units_from = "MiB";
    const char* units_to = "MB";

    const double result = convert_units(from, units_from, units_to);
    const double expected = 5.242880;
    printf("Result  : %f\n", result);
    printf("Expected: %f\n", expected);
    TEST_ASSERT_EQUAL_FLOAT(expected, result);
}

void convert_units_si_to_binary_should_return_correct_conversion() {
    const char* from = "5MB";
    const char* units_from = "MB";
    const char* units_to = "MiB";

    const double result = convert_units(from, units_from, units_to);
    const double expected = 4.768372;
    printf("Result  : %f\n", result);
    printf("Expected: %f\n", expected);
    TEST_ASSERT_EQUAL_FLOAT(expected, result);
}

void convert_units_si_to_si_should_return_correct_conversion() {
    const char* from = "5GiB";
    const char* units_from = "GiB";
    const char* units_to = "MiB";
    const double result = convert_units(from, units_from, units_to);
    const double expected = 5120.00;
    TEST_ASSERT_EQUAL_FLOAT(expected, result);
}

void convert_units_binary_to_binary_should_return_correct_conversion() {
    const char* from = "5GB";
    const char* units_from = "GB";
    const char* units_to = "MB";
    const double result = convert_units(from, units_from, units_to);
    const double expected = 5000.00;
    TEST_ASSERT_EQUAL_FLOAT(expected, result);
}

/* Main runner */
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(calc_factor_si_should_return_correct_bytescale);
    RUN_TEST(calc_factor_si_byte_should_return_correct_bytescale);
    RUN_TEST(calc_factor_binary_should_return_correct_bytescale);
    RUN_TEST(calc_factor_binary_byte_should_return_correct_bytescale);
    RUN_TEST(convert_units_binary_to_si_should_return_correct_conversion);
    RUN_TEST(convert_units_si_to_binary_should_return_correct_conversion);
    RUN_TEST(convert_units_si_to_si_should_return_correct_conversion);
    RUN_TEST(convert_units_binary_to_binary_should_return_correct_conversion);
    return UNITY_END();
}
