#include "bytesize.h"

/* Determine if there are any regex matches with the input text */
const char *match(const char *input, const char *regex) {
  /* for pcre_compile */
  pcre *re;
  const char *error;
  int erroffset;

  /* for pcre_exec */
  int rc;
  int ovector[32];

  /* to get substrings from regex */
  const char *substring;

  /* we'll start after the first quote and chop off the end quote */
  const char *subject = input;

  re = pcre_compile(regex, 0, &error, &erroffset, NULL);
  rc = pcre_exec(re, NULL, subject, strlen(subject), 0, 0, ovector, 30);

  if (rc == PCRE_ERROR_NOMATCH) {
    fprintf(stderr, "no match\n");
    exit(0);
  } else if (rc < -1) {
    fprintf(stderr, "error %d from regex\n",rc);
    exit(rc);
  }

  pcre_get_substring(subject, ovector, rc, 0, &substring);
  pcre_free(re);

  return substring;
}

/* Determine if an element is found in the array of strings */
bool found_in(const char *elem, const char *array[], int array_size) {
  for (int i = 0; i < array_size; i++)
    if (smatch(elem, array[i]))
      return true;
  return false;
}

/* Determine if a string is equal to some pattern */
bool smatch(const char* input, const char* pattern) {
  bool is_equal = (strcmp(input, pattern) == 0) ? true : false;
  return is_equal;
}

/* Determine if a specified unit is a byte */
bool is_byte(const char* unit) {
  return smatch(unit, "B");
}

/**
  * Calculate the unit-unit conversion factor given that the unit is
  * in either SI_BYTE or BYTE (but not both).
  */
long int calc_factor(const char *unit, int size, const Scale scale) {
  if (is_byte(unit))
    return 1;
  for (int i = 0; i < size; i++) {
    const long int factor = pow(scale.scale, i + 1);
    if (smatch(unit, scale.sizes[i]))
      return factor;
  }
  return 0;
}

/* Determines which byte scaling (binary, or si) to use for the unit */
Scale get_scale(const char *unit) {
  Scale scale = (found_in(unit, SI_BYTE, SIZE)) ? SI : BINARY;
  return scale;
}

/* Determine the relative scaling of a unit with respect to a binary or si byte */
long int get_factor(const char *unit) {
  Scale scale = get_scale(unit);
  long int factor = calc_factor(unit, SIZE, scale);
  return factor;
}

/* Formats and displays the converted size */
void display_units(mpfr_t conversion, const char* units, bool show_with_units) {
  mpfr_t res, divisor, r1;
  mpfr_inits2(200, res, divisor, r1, NULL);

  mpfr_set_ui(divisor, 2, MPFR_RNDF);

  /* Display only whole numbers if the result is exact */
  mpfr_set(r1, conversion, MPFR_RNDF);
  mpfr_modf(res, r1, divisor, MPFR_RNDZ);
  if (mpfr_cmp_ui(r1, 0) == 0) {
    mpz_t int_conv;
    mpz_init2(int_conv, 200);
    mpfr_get_z(int_conv, conversion, MPFR_RNDF);
    (show_with_units) ? mpfr_printf("%Zd %s\n", int_conv, units) : mpfr_printf("%Zd\n", int_conv);
    mpz_clear (int_conv);
  }
  else
    (show_with_units) ? mpfr_printf("%.2Rf %s\n", conversion, units) : mpfr_printf("%.2Rf\n", conversion);

  mpfr_clears(res, divisor, r1, NULL);
  mpfr_free_cache2(MPFR_FREE_LOCAL_CACHE);
}

/** Returns the byte size unit
  * Note that this value must be explcitly deallocated later with
  * `pcre_free_substring()`
  */
const char* get_unit(const char *input) {
  const char *units = match(input, unit_regex);
  return units;
}

/** Parses the input and returns the numeral value as a strings.
  * Note that this value must be explcitly deallocated later with
  * `pcre_free_substring()`
  */
const char* get_amt(const char* input) {
  const char* amt = match(input, num_regex);
  return amt;
}

/* Converts an integral number between byte sizes */
Byte convert_units(char* input, const char* units_from, const char* units_to) {
  mpfr_t from, to, amt, factor;
  /*mpfr_inits2((mpfr_prec_t) 200, from, to, amt, factor, NULL);*/
  mpfr_inits2(200, from, to, amt, factor, NULL);
  /*mpfr_inits2(200, from, to, amt, factor);*/

  const char* digits = get_amt(input);
  mpfr_init_set_str(amt, digits, 10, MPFR_RNDF);  /* const unsigned long long int amt = get_amt(input); */
  mpfr_set_ui(from, get_factor(units_from), MPFR_RNDF);   /* const long int from = get_factor(units_from); */
  mpfr_set_ui(to, get_factor(units_to), MPFR_RNDF);       /* const long int to = get_factor(units_to); */

  char* b1;
  char* b2;
  char* b3;
  mpfr_asprintf(&b1, "Amount To Convert      : %Rf", amt);
  log_debug(b1);

  mpfr_asprintf(&b2, "Conversion Factor From : %Rf", from);
  log_debug(b2);

  mpfr_asprintf(&b3, "Conversion Factor To   : %Rf", to);
  log_debug(b3);

  /*log_debug("Amount To Convert      : %d", amt);*/
  /*log_debug("Conversion Factor From : %ld", from);*/
  /*log_debug("Conversion Factor To   : %ld", to);*/

  /*const long double factor = (double) from / to;*/
  mpfr_div(factor, from, to, MPFR_RNDF);

  /*const long double conversion = amt * factor;*/
  mpfr_mul(to, amt, factor, MPFR_RNDF);

  Byte byte = {{(mpfr_prec_t) 0}, units_to, 0};
  mpfr_init2 (byte.amt, 200);
  mpfr_set(byte.amt, to, MPFR_RNDF);

  mpfr_free_str(b1);
  mpfr_free_str(b2);
  mpfr_free_str(b3);
  pcre_free_substring(digits);
  mpfr_clears(from, to, amt, factor, NULL);
  mpfr_free_cache2(MPFR_FREE_LOCAL_CACHE);

  return byte;
}

/* Automatically determines the best size to use
   and returns the converted unit */
Byte auto_size(mpfr_t bytes, int scale, bool is_byte) {
  mpfr_t bscale, r1, r2, r3;
  mpfr_inits2(200, bscale, r1, r2, r3, NULL);

  mpfr_set_ui(bscale, (unsigned int) scale, MPFR_RNDF);

  /* i = floor (log(bytes) / log(scale)) */
  mpfr_log(r1, bytes, MPFR_RNDD);
  mpfr_log(r2, bscale, MPFR_RNDD);
  mpfr_div(r3, r1, r2, MPFR_RNDD);
  mpfr_floor(r3, r3);
  
  /* long double amt = (long double) bytes / (long double) pow(scale, i) */
  mpfr_pow(r1, bscale, r3, MPFR_RNDF);  /* pow(scale, i) */
  mpfr_div(r2, bytes, r1, MPFR_RNDF);   /* amt = bytes / pow(scale, i) */

  /* If the output size is < than 1KB or 1KiB (depends on scale), display in bytes */
  int i = mpfr_get_ui (r3, MPFR_RNDD);
  i = (is_byte) ? i - 1: i;
  char* unit = (scale == SI_SCALE) ? SI_BYTE[i]: BYTE[i];
  unit = (i < 0) ? "B" : unit;

  Byte byte = { {(mpfr_prec_t) 0}, unit, 0};
  mpfr_init2 (byte.amt, 200);
  mpfr_set(byte.amt, r2, MPFR_RNDF);

  // Deallocate
  mpfr_clears(bytes, bscale, r1, r2, r3, NULL);
  mpfr_free_cache2(MPFR_FREE_LOCAL_CACHE);
  mpfr_mp_memory_cleanup();
  
  if (i >= SIZE) {
    puts("Conversion exceeds maximum unit available (YB, YiB). Exiting...");
    exit(-1);
  } else if (i < -1) {
    puts("Conversion exceeds minimum unit available (B). Exiting...");
    exit(-2);
  }

  return byte;
}
