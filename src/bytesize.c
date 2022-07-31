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
long double convert_units(char* input, const char* units_from, const char* units_to) {
  const long int from = get_factor(units_from);
  const long int to = get_factor(units_to);
  const unsigned long long int amt = get_amt(input);
  log_debug("Amount To Convert      : %d", amt);
  log_debug("Conversion Factor From : %ld", from);
  log_debug("Conversion Factor To   : %ld", to);

  const long double factor = (double) from / to;
  const long double conversion = amt * factor;
  return conversion;
}

double bigint_log(mpz_t x) {
  signed long int ex;
  const double di = mpz_get_d_2exp(&ex, x);
  return log(di) + log(2) * (double) ex;
}

/* Automatically determines the best size to use
   and returns the converted unit */
/*Byte auto_size(unsigned long long int bytes, size_t scale, bool is_byte) {*/
/*Byte auto_size(mpz_t bytes, size_t scale, bool is_byte) {*/
Byte auto_size(mpfr_t bytes, size_t scale, bool is_byte) {
  /**
    * mpz_t bytes, mpz_t scale, mpz_t i, mpz_t res
    * Operations:
    * 1. i = floor (log(bytes) / log(scale))
    * - res = mpz_log (scale)
    */
  /*long int i = floor(bigint_log(bytes) / log(scale));*/

  /* Initialize the bigscale */
  mpfr_t bscale;
  mpfr_init2 (bscale, 200);
  mpfr_set_ui(bscale, (unsigned int) scale, MPFR_RNDF);

  /* i = floor (log(bytes) / log(scale)) */
  mpfr_t r1, r2, r3;
  mpfr_init2 (r1, 200);
  mpfr_init2 (r2, 200);
  mpfr_init2 (r3, 200);

  

  /*mpfr_set_ui(r1, (unsigned int) 0, MPFR_RNDF);*/
  /*mpfr_set_ui(r2, (unsigned int) 0, MPFR_RNDF);*/
  /*mpfr_set_ui(r3, (unsigned int) 0, MPFR_RNDF);*/

  /*mpfr_log(r1, bytes, MPFR_RNDZ);*/
  /*mpfr_log(r2, bscale, MPFR_RNDZ);*/
  /*mpfr_div(r3, r1, r2, MPFR_RNDZ); [> i = r3 <]*/

  /*mpfr_log(r1, bytes, MPFR_RNDF);*/
  /*mpfr_log(r2, bscale, MPFR_RNDF);*/
  /*mpfr_div(r3, r1, r2, MPFR_RNDF); [> i = r3 <]*/

  /* Working */
  /*mpfr_log(r1, bytes, MPFR_RNDF);*/
  /*mpfr_log(r2, bscale, MPFR_RNDF);*/
  /*mpfr_div(r3, r1, r2, MPFR_RNDF); [> i = r3 <]*/

  mpfr_log(r1, bytes, MPFR_RNDD);
  mpfr_log(r2, bscale, MPFR_RNDD);
  mpfr_div(r3, r1, r2, MPFR_RNDD); /* i = r3 */



  mpfr_floor(r3, r3);
  
  /* long double amt = (long double) bytes / (long double) pow(scale, i) */
  mpfr_t r4, r5;
  mpfr_init2 (r4, 200);
  mpfr_init2 (r5, 200);

  /*mpfr_pow(r4, bscale, r3, MPFR_RNDZ); [> pow(scale, i) <]*/
  mpfr_pow(r4, bscale, r3, MPFR_RNDF); /* pow(scale, i) */
  /*mpfr_div(r5, bytes, r4, MPFR_RNDZ); [> amt = (long double) bytes / pow(scale, i)<]*/
  mpfr_div(r5, bytes, r4, MPFR_RNDF); /* amt = (long double) bytes / pow(scale, i)*/

  /*long int i = floor(mpfr_log(bytes, 10, MPFR_RNDZ) / log(scale));*/

  /* Initialization */
  /*mpfr_t bigscale;*/
  /*mpfr_init2(t, mp);*/
  /*mpfr_t res;*/
  /*mpfr_log(res, bytes, MPFR_RNDZ)*/


  /*mpz_t amt, bigscale, i;*/

  /*int i = floor(log(bytes) / log(scale)); */
  /*long double amt = (long double) bytes / (long double) pow(scale, i); */
  /*mpz_t amt, bigscale, i;*/
  /*mpf_t amt;*/
  /*mpf_t i;*/
  /*mpf_t i;*/
  /*mpz_pow_ui(i, scale, */

  /*i = (is_byte) ? i - 1 : i;*/
  mpfr_t index;
  mpfr_init2 (index, 200);
  /*unsigned long i = 0;*/
  /*int i = 0;*/
  /*int i = 0;*/
  /*int i = mpfr_get_ui (index, MPFR_RNDD);*/
  int i = mpfr_get_ui (r3, MPFR_RNDD);
  i = (is_byte) ? i - 1: i;
  /*if (is_byte)*/
    /*mpfr_sub_si(index, r3, (long) 1, MPFR_RNDZ);*/
    /*mpfr_sub_si(index, r3, (long) 1, MPFR_RNDD);*/
  /*{*/
     /*i--;*/


    /*puts("Doing substraction");*/
    /*printf("Before: %ld\n", mpfr_get_ui (r3, MPFR_RNDD));*/
    /*int res = mpfr_sub_ui(index, r3, 2, MPFR_RNDD);*/
    /*long int subtrahend = 3000;*/
    /*int res = mpfr_sub_ui(index, r3, subtrahend, MPFR_RNDD);*/
    /*int res = mpfr_sub_si(index, r3, subtrahend, MPFR_RNDD);*/

    /*mpfr_sub_si(index, index, (long) 1, MPFR_RNDZ);*/
    /*printf("After: %ld\n", mpfr_get_ui (index, MPFR_RNDD));*/
    /*printf("Result Code: %d\n", res);*/
  /*}*/
  /*else */
    /*mpfr_set(index, r3, MPFR_RNDD);*/

  printf("%ld \n", i);
  /*printf("%d \n", i);*/
  char* unit = (scale == SI_SCALE) ? SI_BYTE[i]: BYTE[i];
  /*printf("Before: %s \n", unit);*/
  /*unit = (i < 0) ? "B" : unit;*/

  unit = (i < 0) ? "B" : unit;

  printf("After: %s \n", unit);

  /*Byte byte = {amt, unit};*/
  /*Byte byte = {&r5, unit, scale};*/
  /*Byte byte = {*r5, unit, scale};*/
  /*Byte byte = {r5, unit, scale};*/
  /*Byte byte = {(mpfr_ptr) 0, unit, scale};*/

  /*Byte byte = { {r5}, unit, 0};*/
  Byte byte = { {(mpfr_ptr) 0}, unit, 0};

  /*printf("%s \n", byte.unit);*/
  mpfr_init2 (byte.amt, 200);
  /*mpfr_set(byte.amt, r5, MPFR_RNDZ);*/
  mpfr_set(byte.amt, r5, MPFR_RNDF);

  // Deallocate
  mpfr_clear (r1);
  mpfr_clear (r2);
  mpfr_clear (r3);
  mpfr_clear (r4);
  /*mpfr_clear (r5);*/
  mpfr_clear (index);
  mpfr_free_cache();
  
  if (i >= SIZE) {
    puts("Conversion exceeds maximum unit available (YB, YiB). Exiting...");
    exit(-1);
  } else if (i < 0) {
    puts("Conversion exceeds minimum unit available (B). Exiting...");
    exit(-2);
  }

  return byte;
}
