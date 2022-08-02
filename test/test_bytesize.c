#include "bytesize.h"
#include "unity.h"


void setUp(void) {}

void tearDown(void) {}

/* Test Helper Functions */
void compare_bytes(Byte to, const double expected) {
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);
  printf("Result  : %f\n", result);
  printf("Expected: %f\n", expected);
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  /* Deallocate */
  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_LOCAL_CACHE);
}

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

void calc_factor_returns_correct_scale() {
  const char* si = "KB";
  const char* bi = "KiB";
  const char* bytes = "B";

  TEST_ASSERT_EQUAL_INT(1000, calc_factor(si, SIZE, SI));         /* 1 KB   == 1000 B */
  TEST_ASSERT_EQUAL_INT(1024, calc_factor(bi, SIZE, BINARY));     /* 1 KiB  == 1024 B */
  TEST_ASSERT_EQUAL_INT(1   , calc_factor(bytes, SIZE, SI));      /* 1 B    == 1 B    */
  TEST_ASSERT_EQUAL_INT(1   , calc_factor(bytes, SIZE, BINARY));  /* 1B is the same for SI, Binary units */
}

void convert_units_returns_correct_conversions() {
  const char* si = "5MB";
  const char* bi = "5MiB";
  char* units_from_si = get_unit(si);
  char* units_from_bi = get_unit(bi);
  double expected;
  Byte to;

  /* MB->MiB (SI->Binary) */
  to = convert_units("5", units_from_si, "MiB");
  expected = 4.768372;
  compare_bytes(to, expected);

  /* MiB->MB (Binary->SI) */
  to = convert_units("5", units_from_bi, "MB");
  expected = 5.242880;
  compare_bytes(to, expected);

  /* GiB->MiB (Binary->Binary) */
  to = convert_units("5", "GiB", "MiB");
  expected = 5120.00;
  compare_bytes(to, expected);

  /* GB->MB (SI->SI) */
  to = convert_units("5", "GB", "MB");
  expected = 5000.00;
  compare_bytes(to, expected);

  /* Bytes */
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
  RUN_TEST(calc_factor_returns_correct_scale);
  // Normal convert_units tests
  RUN_TEST(convert_units_returns_correct_conversions);

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
