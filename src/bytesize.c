#include "bytesize.h"

/*! Determine if a specified unit is a byte */
bool is_byte(const char* unit) {
  return smatch(unit, "B");
}

/** Determines if the big decimal value is a whole number */
bool is_integral(mpfr_t conversion) {
  mpfr_t res, divisor, r1;
  mpfr_inits2(200, res, divisor, r1, NULL);

  mpfr_set_ui(divisor, 2, MPFR_RNDF);

  /* Display only whole numbers if the result is exact */
  mpfr_set(r1, conversion, MPFR_RNDF);
  mpfr_modf(res, r1, divisor, MPFR_RNDZ);
  /* If the result is divisible by 2,
     and the conversion amount is greater than zero, display as a whole number.
     Else display as a scientific number */
  bool is_integral = ((mpfr_cmp_ui(r1, 0) == 0) && (mpfr_cmp_ui(res, 0) > 0)) ? true : false;

  /* Deallocate */
  mpfr_clears(res, divisor, r1, NULL);
  mpfr_free_cache2(MPFR_FREE_LOCAL_CACHE);

  return is_integral;
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

/*! Determines which byte scaling (binary, or si) to use for the unit */
Scale get_scale(const char *unit) {
  Scale scale = (found_in(unit, SI_BYTE, SIZE)) ? SI : BINARY;
  return scale;
}

/*! Determine the relative scaling of a unit with respect to a binary or si byte */
long int get_factor(const char *unit) {
  Scale scale = get_scale(unit);
  long int factor = calc_factor(unit, SIZE, scale);
  return factor;
}

/*! Formats and displays the converted size */
void display_units(mpfr_t conversion, const char* units, bool show_with_units) {
  if (is_integral(conversion)) {
    mpz_t int_conv;
    mpfr_to_mpz(int_conv, conversion);
    (show_with_units) ? mpfr_printf("%Zd %s\n", int_conv, units) : mpfr_printf("%Zd\n", int_conv);
    mpz_clear (int_conv);
  }
  else
    (show_with_units) ? mpfr_printf("%Rg %s\n", conversion, units) : mpfr_printf("%Rg\n", conversion);

}

/** Returns the byte size unit
  * Note that this value must be explcitly deallocated later with
  * `pcre_free_substring()` */
const char* get_unit(const char *input) {
  const char *units = match(input, unit_regex);
  return units;
}

/** Parses the input and returns the numeral value as a strings.
  * Note that this value must be explcitly deallocated later with
  * `pcre_free_substring()` */
const char* get_amt(const char* input) {
  const char* amt = match(input, num_regex);
  return amt;
}

/** Formats a big decimal mpfr_t value into a string
  * Note that you must deallocate this string later with `mpfr_free_str()` */
char* fmt_mpfr_str(const char* tformat, mpfr_t amt) {
  char* buffer;
  mpfr_asprintf(&buffer, tformat, amt);
  return buffer;
}

/** Formats a big integer mpz_t value into a string
  * Note that you must deallocate this string later with `mpfr_free_str()` */
char* fmt_mpz_str(const char* tformat, mpz_t amt) {
  char* buffer;
  mpfr_asprintf(&buffer, tformat, amt);
  return buffer;
}

/*! Converts an integral number between byte sizes */
Byte convert_units(const char* digits, const char* units_from, const char* units_to) {
  mpfr_t from, to, amt, factor;
  mpfr_inits2(200, from, to, factor, NULL); /* amt will be initialized later */

  mpfr_init_set_str(amt, digits, 10, MPFR_RNDF);  /* const unsigned long long int amt = get_amt(input); */
  mpfr_set_ui(from, get_factor(units_from), MPFR_RNDF);   /* const long int from = get_factor(units_from); */
  mpfr_set_ui(to, get_factor(units_to), MPFR_RNDF);       /* const long int to = get_factor(units_to); */

  Byte bstrs[] = {
    {{*amt}, "Amount To Convert      : %Rf", 0},
    {{*from}, "Conversion Factor From : %Rf", 0},
    {{*to}, "Conversion Factor To   : %Rf", 0}
  };

  for (int i = 0; i < 3; i++) {
    char* buffer = fmt_mpfr_str(bstrs[i].unit, bstrs[i].amt);
    log_debug(buffer);
    mpfr_free_str(buffer);
  }

  /*const long double factor = (double) from / to;*/
  mpfr_div(factor, from, to, MPFR_RNDF);

  /*const long double conversion = amt * factor;*/
  mpfr_mul(to, amt, factor, MPFR_RNDF);

  Byte byte = {{(mpfr_prec_t) 0}, units_to, 0};
  mpfr_init2 (byte.amt, 200);
  mpfr_set(byte.amt, to, MPFR_RNDF);

  /* Deallocate */
  mpfr_clears(from, to, amt, factor, NULL);
  mpfr_free_cache2(MPFR_FREE_LOCAL_CACHE);

  return byte;
}

/*! Automatically determines the best size to use
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
  
  if (i >= SIZE) {
    puts("Conversion exceeds maximum unit available (YB, YiB). Exiting...");
    exit(-1);
  } else if (i < -1) {
    puts("Conversion exceeds minimum unit available (B). Exiting...");
    exit(-2);
  }

  return byte;
}

/* Automatically determines the best size for a byte string.
   Note that the input is assumed to be a byte string and not
   a string of any other byte size unit of measurement. */
Byte auto_format_bstr(const char* bytes, unsigned int scale) {
  Byte to;

  mpfr_t amt;
  mpfr_init2(amt, 200);
  mpfr_init_set_str(amt, bytes, 10, MPFR_RNDF);
  to = auto_size(amt, scale, true);
  
  /* Deallocate */
  mpfr_clears(amt, NULL);
  mpfr_free_cache2(MPFR_FREE_LOCAL_CACHE);
  return to;
}
