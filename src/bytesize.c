#include "bytesize.h"

/* Determine if there are any regex matches with the input text */
const char *match(char *input, const char *regex) {
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

/* Determine if an element is found in the character array */
bool found_in(const char *elem, const char *array[], int array_size) {
  for (int i = 0; i < array_size; i++)
    if (smatch(elem, array[i]))
      return true;
  return false;
}

/* Determines if a string is equal to some pattern */
bool smatch(const char* input, const char* pattern) {
  bool is_equal = (strcmp(input, pattern) == 0) ? true : false;
  return is_equal;
}

/* Determine if a specified unit is a byte */
bool is_byte(const char* unit) {
  return smatch(unit, "B");
}

/* 
 * Calculates the conversion factor between a unit to another unit
 * given that they are both in either SI_BYTE or the BYTE  arrays.
 * Note that this also means that this does not convert between SI_BYTE to BYTE
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
void display_units(const long double conversion, const char* units, bool show_with_units) {
  if (ceil(conversion) == (unsigned long long int) conversion) 
    (show_with_units) ? printf("%llu %s\n", (unsigned long long int) conversion, units) : printf("%llu\n", (unsigned long long int) conversion);
  else
    (show_with_units) ? printf("%.2Lf %s\n", conversion, units) : printf("%.2Lf\n", conversion);
}

/* Returns the byte size unit */
const char* get_unit(char *input) {
  const char *units = match(input, unit_regex);
  return units;
}

/* Parses the input and returns the numeral value */
unsigned long long int get_amt(char* input) {
  const unsigned long long int amt = strtoull(match(input, num_regex), NULL, 0);
  return amt;
}

/* Converts an integral number between byte sizes */
/*long double convert_units(char* input, const char* units_from, const char* units_to) {*/
Byte convert_units(char* input, const char* units_from, const char* units_to) {
  mpfr_t bfrom, bto, bamt, bfactor;
  const char* digits = match(input, num_regex);

  mpfr_init2 (bfrom, 200);
  mpfr_init2 (bto, 200);
  mpfr_init2 (bamt, 200);
  mpfr_init2 (bfactor, 200);
  
  mpfr_init_set_str(bamt, digits, 10, MPFR_RNDF);
  mpfr_set_ui(bfrom, get_factor(units_from), MPFR_RNDF);
  mpfr_set_ui(bto, get_factor(units_to), MPFR_RNDF);
  /*const long int from = get_factor(units_from);*/
  /*const long int to = get_factor(units_to);*/

  /*const unsigned long long int amt = get_amt(input);*/

  /*log_debug("Amount To Convert      : %d", amt);*/
  /*log_debug("Conversion Factor From : %ld", from);*/
  /*log_debug("Conversion Factor To   : %ld", to);*/
  log_debug("Amount To Convert      : %d", bamt);
  log_debug("Conversion Factor From : %ld", bfrom);
  log_debug("Conversion Factor To   : %ld", bto);

  /*const long double factor = (double) from / to;*/
  mpfr_t r1;
  mpfr_init2 (r1, 200);
  mpfr_div(r1, bfrom, bto, MPFR_RNDF);

  /*const long double conversion = amt * factor;*/
  mpfr_t r2;
  mpfr_init2 (r2, 200);
  mpfr_mul(r2, bamt, r1, MPFR_RNDF);

  Byte byte = { {(mpfr_ptr) 0}, units_to, 0};
  mpfr_init2 (byte.amt, 200);
  mpfr_set(byte.amt, r2, MPFR_RNDF);

  mpfr_clear (bfrom);
  mpfr_clear (bto);
  mpfr_clear (bamt);
  mpfr_clear (bfactor);
  mpfr_free_cache();

  /*return conversion;*/
  return byte;
}

double bigint_log(mpz_t x) {
  signed long int ex;
  const double di = mpz_get_d_2exp(&ex, x);
  return log(di) + log(2) * (double) ex;
}

/* Automatically determines the best size to use
   and returns the converted unit */
Byte auto_size(mpfr_t bytes, size_t scale, bool is_byte) {
  mpfr_t bscale;
  mpfr_init2 (bscale, 200);
  mpfr_set_ui(bscale, (unsigned int) scale, MPFR_RNDF);

  /* i = floor (log(bytes) / log(scale)) */
  mpfr_t r1, r2, r3;
  mpfr_init2 (r1, 200);
  mpfr_init2 (r2, 200);
  mpfr_init2 (r3, 200);

  mpfr_log(r1, bytes, MPFR_RNDD);
  mpfr_log(r2, bscale, MPFR_RNDD);
  mpfr_div(r3, r1, r2, MPFR_RNDD);
  mpfr_floor(r3, r3);
  
  /* long double amt = (long double) bytes / (long double) pow(scale, i) */
  mpfr_t r4, r5;
  mpfr_init2 (r4, 200);
  mpfr_init2 (r5, 200);

  mpfr_pow(r4, bscale, r3, MPFR_RNDF);  /* pow(scale, i) */
  mpfr_div(r5, bytes, r4, MPFR_RNDF);   /* amt = bytes / pow(scale, i) */

  /** If the output size is < than 1KB or 1KiB (depends on scale), display in bytes */
  int i = mpfr_get_ui (r3, MPFR_RNDD);
  i = (is_byte) ? i - 1: i;
  char* unit = (scale == SI_SCALE) ? SI_BYTE[i]: BYTE[i];
  unit = (i < 0) ? "B" : unit;

  Byte byte = { {(mpfr_ptr) 0}, unit, 0};
  mpfr_init2 (byte.amt, 200);
  mpfr_set(byte.amt, r5, MPFR_RNDF);

  // Deallocate
  mpfr_clear (r1);
  mpfr_clear (r2);
  mpfr_clear (r3);
  mpfr_clear (r4);
  mpfr_clear (r5);
  mpfr_free_cache();
  
  if (i >= SIZE) {
    puts("Conversion exceeds maximum unit available (YB, YiB). Exiting...");
    exit(-1);
  } else if (i < -1) {
    puts("Conversion exceeds minimum unit available (B). Exiting...");
    exit(-2);
  }

  return byte;
}
