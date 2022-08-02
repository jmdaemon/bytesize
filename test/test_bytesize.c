#include "bytesize.h"
#include "unity.h"


void setUp(void) {}

void tearDown(void) {}

void match_returns_correct_substrings() {
  const char* si = "5MB";
  const char* bi = "5MiB";
  const char* bytes = "5B";

  /* num_regex returns digits */
  TEST_ASSERT_EQUAL_STRING("5", match(si, num_regex));
  TEST_ASSERT_EQUAL_STRING("5", match(bytes, num_regex));
  /* unit returns units for si or binary units */
  TEST_ASSERT_EQUAL_STRING("MB", match(si, unit_regex));
  TEST_ASSERT_EQUAL_STRING("MiB", match(bi, unit_regex));
}

// calc_factor tests
void calc_factor_si_should_return_correct_bytescale() {
  // The scale is with the respect of the unit to the scale of one byte
  // measurement
  const char *unit = "KB";
  const long int expected = 1000;
  const long int result = calc_factor(unit, SIZE, SI);
  // 1 KB == 1000 Bytes
  TEST_ASSERT_EQUAL_INT(expected, result);
}

void calc_factor_binary_should_return_correct_bytescale() {
  // The scale is with the respect of the unit to the scale of one byte
  // measurement
  const char *unit = "KiB";
  const long int expected = 1024;
  const long int result = calc_factor(unit, SIZE, BINARY);
  TEST_ASSERT_EQUAL_INT(expected, result);
}

void calc_factor_byte_should_return_correct_bytescale() {
  // 1 B == 1 B
  const char *unit = "B";
  const long int expected = 1;
  const long int binary_byte = calc_factor(unit, SIZE, BINARY);
  const long int si_byte = calc_factor(unit, SIZE, SI);
  TEST_ASSERT_EQUAL_INT(expected, binary_byte);
  TEST_ASSERT_EQUAL_INT(expected, si_byte);
}

// Normal unit conversions
void convert_units_binary_to_si_should_return_correct_conversion() {
  const char *from = "5MiB";
  const char *units_from = "MiB";
  const char *units_to = "MB";
  const char *digits = "5";

  Byte to = convert_units(digits, units_from, units_to);

  /*const double result = convert_units(from, units_from, units_to);*/
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);
  const double expected = 5.242880;
  printf("Result  : %f\n", result);
  printf("Expected: %f\n", expected);
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
}

void convert_units_si_to_binary_should_return_correct_conversion() {
  const char *from = "5MB";
  const char *units_from = "MB";
  const char *units_to = "MiB";
  const char *digits = "5";

  Byte to = convert_units(digits, units_from, units_to);

  /*const double result = convert_units(from, units_from, units_to);*/
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);

  const double expected = 4.768372;
  printf("Result  : %f\n", result);
  printf("Expected: %f\n", expected);
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
}

void convert_units_si_to_si_should_return_correct_conversion() {
  const char *from = "5GiB";
  const char *units_from = "GiB";
  const char *units_to = "MiB";
  const char *digits = "5";

  Byte to = convert_units(digits, units_from, units_to);

  /*const double result = convert_units(from, units_from, units_to);*/
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);
  const double expected = 5120.00;
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
}

void convert_units_binary_to_binary_should_return_correct_conversion() {
  const char *from = "5GB";
  const char *units_from = "GB";
  const char *units_to = "MB";
  const char *digits = "5";

  Byte to = convert_units(digits, units_from, units_to);
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);

  const double expected = 5000.00;
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
}

// Byte conversions

// Byte to Byte
void convert_units_byte_to_byte_should_return_correct_conversion() {
  const char *from = "5B";
  const char *units_from = "B";
  const char *units_to = "B";
  /*const double result = convert_units(from, units_from, units_to);*/
  const char *digits = "5";

  Byte to = convert_units(digits, units_from, units_to);
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);

  const double expected = 5;
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
}

// Byte to SI
void convert_units_byte_to_si_should_return_correct_conversion() {
  const char *from = "5B";
  const char *units_from = "B";
  const char *units_to = "KB";
  /*const double result = convert_units(from, units_from, units_to);*/
  const char *digits = "5";

  Byte to = convert_units(digits, units_from, units_to);
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);

  const double expected = 0.005;
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
}

// Byte to Binary
void convert_units_byte_to_binary_should_return_correct_conversion() {
  const char *from = "5B";
  const char *units_from = "B";
  const char *units_to = "KiB";
  /*const double result = convert_units(from, units_from, units_to);*/
  const char *digits = "5";

  Byte to = convert_units(digits, units_from, units_to);
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);

  const double expected = 0.00488281;
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
}

// SI to Byte
void convert_units_si_to_byte_should_return_correct_conversion() {
  const char *from = "5MB";
  const char *units_from = "MB";
  const char *units_to = "B";
  /*const double result = convert_units(from, units_from, units_to);*/
  const char *digits = "5";

  Byte to = convert_units(digits, units_from, units_to);
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);

  const double expected = 5000000;
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
}

// Binary to Byte
void convert_units_binary_to_byte_should_return_correct_conversion() {
  const char *from = "5MiB";
  const char *units_from = "MiB";
  const char *units_to = "B";
  /*const double result = convert_units(from, units_from, units_to);*/
  const char *digits = "5";

  Byte to = convert_units(digits, units_from, units_to);
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);

  const double expected = 5242880;
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
}

// Big Byte Tests
void convert_units_petabyte_to_byte_should_return_correct_conversion() {
  const char *from = "5PB";
  const char *units_from = "PB";
  const char *units_to = "B";
  /*const double result = convert_units(from, units_from, units_to);*/
  const char *digits = "5";

  Byte to = convert_units(digits, units_from, units_to);
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);

  const long double expected = 5000000000000000.00;
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
}

/* Main runner */
int main(void) {
  UNITY_BEGIN();
  RUN_TEST(match_returns_correct_substrings);
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
  return UNITY_END();
}
