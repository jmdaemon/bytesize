#include "bytesize.h"
#include "unity.h"

/* Helpers */
typedef struct TestByte {
  const char* amt;
  const char* from;
  const char* to;
  const double expected;
} TestByte;

void compare_bytes(Byte to, const double expected) {
  const double result = mpfr_get_d(to.amt, MPFR_RNDF);
  printf("Result  : %f\n", result);
  printf("Expected: %f\n", expected);
  TEST_ASSERT_EQUAL_FLOAT(expected, result);

  /* Deallocate */
  mpfr_clears(to.amt, NULL);
  mpfr_free_cache2(MPFR_FREE_LOCAL_CACHE);
}

/* Test Fixture Setup/Teardown */
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
  const char* amt = "5";
  const unsigned int CONVERSIONS_TEST_SIZE = 9;
  TestByte conversions[] = {
    {amt, "MB"  , "MiB" , 4.768372},  /* MB->MiB (SI->Binary) */
    {amt, "MiB" , "MB"  , 5.242880},  /* MiB->MB (Binary->SI) */
    {amt, "GiB" , "MiB" , 5120.00},   /* GiB->MiB (Binary->Binary) */
    {amt, "GB"  , "MB"  , 5000.00},   /* GB->MB (SI->SI) */
    /* Byte Conversions */
    {amt, "B"   , "B"   , 5.0},       /* B->B */
    {amt, "B"   , "KB"  , 0.005},     /* B->KB (B->SI) */
    {amt, "B"   , "KiB" , 0.00488281},/* B->KiB (B->Binary) */
    {amt, "MB"  , "B"   , 5000000.0}, /* MB->B (SI->B) */
    {amt, "MiB" , "B"   , 5242880.0}, /* MiB->B (Binary->B) */
  };
  Byte to;

  /* Easily test all of our conversions */
  for (int i = 0; i < CONVERSIONS_TEST_SIZE; i++) {
    TestByte test = conversions[i];
    to = convert_units(test.amt, test.from, test.to);
    compare_bytes(to, test.expected);
  }
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
  RUN_TEST(convert_units_returns_correct_conversions);
  // Big number conversion tests
  RUN_TEST(convert_units_petabyte_to_byte_should_return_correct_conversion);
  return UNITY_END();
}
