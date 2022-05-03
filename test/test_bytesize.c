#include "bytesize.h"
#include "unity.h"

void setUp(void) {
    /* set stuff up here */
}

void tearDown(void) {
    /* clean stuff up here */
}

void num_regex_should_return_num() {
    const char* input = "5MB";
    const char* result = match(input, num_regex);
    const char* expected = "5";
    TEST_ASSERT_EQUAL_STRING(expected, result);
}

// get_unit tests
void get_unit_si_should_return_si_unit() {
    const char* input = "5MB";
    const char* result = get_unit(input);
    const char* expected = "MB";
    TEST_ASSERT_EQUAL_STRING(expected, result);
}
void get_unit_binary_should_return_binary_unit() {
    const char* input = "5MiB";
    const char* result = get_unit(input);
    const char* expected = "MiB";
    TEST_ASSERT_EQUAL_STRING(expected, result);
}
void get_unit_byte_should_return_byte_unit() {
    const char* input = "5B";
    const char* result = get_unit(input);
    const char* expected = "B";
    TEST_ASSERT_EQUAL_STRING(expected, result);
}

// calc_factor tests
void calc_factor_si_should_return_correct_bytescale() {
    // The scale is with the respect of the unit to the scale of one byte measurement
    const char* unit = "KB";
    const long int expected = 1000;
    const long int result = calc_factor(unit, SIZE, SI_BYTE, SI_SCALE);
    // 1 KB == 1000 Bytes
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

void calc_factor_byte_should_return_correct_bytescale() {
    // 1 B == 1 B
    const char* unit = "B";
    const long int expected = 1;
    const long int binary_byte = calc_factor(unit, SIZE, BYTE, BINARY_SCALE);
    const long int si_byte = calc_factor(unit, SIZE, SI_BYTE, SI_SCALE);
    TEST_ASSERT_EQUAL_INT(expected, binary_byte);
    TEST_ASSERT_EQUAL_INT(expected, si_byte);
}

// Normal unit conversions
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

// Byte conversions

// Byte to Byte
void convert_units_byte_to_byte_should_return_correct_conversion() {
    const char* from = "5B";
    const char* units_from = "B";
    const char* units_to = "B";
    const double result = convert_units(from, units_from, units_to);
    const double expected = 5;
    TEST_ASSERT_EQUAL_FLOAT(expected, result);
}

// Byte to SI
void convert_units_byte_to_si_should_return_correct_conversion() {
    const char* from = "5B";
    const char* units_from = "B";
    const char* units_to = "KB";
    const double result = convert_units(from, units_from, units_to);
    const double expected = 0.005;
    TEST_ASSERT_EQUAL_FLOAT(expected, result);
}

// Byte to Binary
void convert_units_byte_to_binary_should_return_correct_conversion() {
    const char* from = "5B";
    const char* units_from = "B";
    const char* units_to = "KiB";
    const double result = convert_units(from, units_from, units_to);
    const double expected = 0.00488281;
    TEST_ASSERT_EQUAL_FLOAT(expected, result);
}

// SI to Byte
void convert_units_si_to_byte_should_return_correct_conversion() {
    const char* from = "5MB";
    const char* units_from = "MB";
    const char* units_to = "B";
    const double result = convert_units(from, units_from, units_to);
    const double expected = 5000000;
    TEST_ASSERT_EQUAL_FLOAT(expected, result);
}

// Binary to Byte
void convert_units_binary_to_byte_should_return_correct_conversion() {
    const char* from = "5MiB";
    const char* units_from = "MiB";
    const char* units_to = "B";
    const double result = convert_units(from, units_from, units_to);
    const double expected = 5242880;
    TEST_ASSERT_EQUAL_FLOAT(expected, result);
}

// Big Byte Tests

void convert_units_petabyte_to_byte_should_return_correct_conversion() {
    const char* from = "5PB";
    const char* units_from = "PB";
    const char* units_to = "B";
    const double result = convert_units(from, units_from, units_to);
    const long double expected = 5000000000000000.00;
    TEST_ASSERT_EQUAL_FLOAT(expected, result);
}

/*void convert_units_yetabyte_to_byte_should_return_correct_conversion() {*/
    /*const char* from = "5YB";*/
    /*const char* units_from = "YB";*/
    /*const char* units_to = "B";*/
    /*const double result = convert_units(from, units_from, units_to);*/
    /*const long double expected = 5000000000000000.00;*/
    /*TEST_ASSERT_EQUAL_FLOAT(expected, result);*/
/*}*/

/* Main runner */
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(num_regex_should_return_num);
    // get_unit tests
    RUN_TEST(get_unit_si_should_return_si_unit);
    RUN_TEST(get_unit_binary_should_return_binary_unit);
    RUN_TEST(get_unit_byte_should_return_byte_unit);
    // calc_factor tests
    RUN_TEST(calc_factor_si_should_return_correct_bytescale);
    RUN_TEST(calc_factor_binary_should_return_correct_bytescale);
    RUN_TEST(calc_factor_byte_should_return_correct_bytescale);
    // Normal convert_units tests
    RUN_TEST(convert_units_si_to_si_should_return_correct_conversion);
    RUN_TEST(convert_units_si_to_binary_should_return_correct_conversion);
    RUN_TEST(convert_units_binary_to_binary_should_return_correct_conversion);
    RUN_TEST(convert_units_binary_to_si_should_return_correct_conversion);
    // Byte Conversion tests
    RUN_TEST(convert_units_byte_to_byte_should_return_correct_conversion);
    RUN_TEST(convert_units_byte_to_si_should_return_correct_conversion);
    RUN_TEST(convert_units_byte_to_binary_should_return_correct_conversion);
    RUN_TEST(convert_units_si_to_byte_should_return_correct_conversion);
    RUN_TEST(convert_units_binary_to_byte_should_return_correct_conversion);
    // Big number conversion tests
    RUN_TEST(convert_units_petabyte_to_byte_should_return_correct_conversion);
    /*RUN_TEST(convert_units_yetabyte_to_byte_should_return_correct_conversion);*/
    return UNITY_END();
}
