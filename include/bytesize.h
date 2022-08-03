#pragma once
#ifndef BYTESIZE_H
#define BYTESIZE_H

/* Support header include for C++ */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pcre.h>
#include <math.h>
#include <limits.h>

/* Third Party Libraries */
#include <gmp.h>
#include <mpfr.h>
#include "log.h"

/** Macros */

#define SIZE 8 /**< Size of the SI_BYTE, BYTE format unit arrays */

/** Create an mpz_t (big integer) version of an mpfr (big decimal) number */
#define mpfr_to_mpz(int_conv, conversion) { \
  mpz_init2(int_conv, 200); \
  mpfr_get_z(int_conv, conversion, MPFR_RNDF); \
}

/* https://en.wikipedia.org/wiki/Binary_prefix */
static const int SI_SCALE = 1000;     /**< 1KB   == 1000 B */
static const int BINARY_SCALE = 1024; /**< 1KiB  == 1024 B */

static const char *unit_regex = "([KMGTPEZY]?i?B+)";  /**< Match with any SI or binary byte */

/* Alternative more resilient regex */
// static const char *num_regex = "[\\d]{1,99}([.]\\d{1,99})?"; /**< Match any numeral digit */
static const char *num_regex = "[\\d]+([.]\\d+)?"; /**< Match any numeral digit */

static const char *SI_BYTE[SIZE] = {
  "KB",
  "MB",
  "GB",
  "TB",
  "PB",
  "EB",
  "ZB",
  "YB"
}; /**< SI byte unit formats */

static const char *BYTE[SIZE] = {
  "KiB",
  "MiB",
  "GiB",
  "TiB",
  "PiB",
  "EiB",
  "ZiB",
  "YiB"
}; /**< Binary byte unit formats */

/** Represents units of bytes in either binary or SI unit systems */
typedef struct Byte {
  mpfr_t amt;         /**< Stores some amount of bytes */
  char* unit;         /**< Stores the byte size unit */
  long int scaling;   /**< Stores the scaling used (SI_SCALE, BINARY_SCALE) */
} Byte; 

/** Represents either SI or binary byte unit scales */
typedef struct Scale {
  const int scale;    /**< The number of bytes in 1 KB or KiB */
  const char** sizes; /**< The byte formats used specific to binary or SI unit systems */
} Scale;

static const struct Scale BINARY = { BINARY_SCALE, BYTE };
static const struct Scale SI = { SI_SCALE, SI_BYTE };

typedef struct Conversion {
  Byte from;
  Byte to;
} Conversion;

/* Helper Functions */
const char *match(const char *input, const char *regex);
bool found_in(const char *elem, const char *array[], int array_size);
bool smatch(const char* input, const char* pattern);

/* Bytesize Functions */
bool is_byte(const char* unit);
bool is_integral(mpfr_t conversion);
Scale get_scale(const char *unit);
long int calc_factor(const char *unit, int size, const Scale scale);
long int get_factor(const char *unit);
void display_units(mpfr_t conversion, const char* units, bool show_with_units);
const char* get_unit(const char *input);
const char* get_amt(const char *input);
char* fmt_mpfr_str(const char* tformat, mpfr_t amt);
char* fmt_mpz_str(const char* tformat, mpz_t amt);
Byte convert_units(const char* digits, const char* units_from, const char* units_to);
Byte auto_size(mpfr_t bytes, int scale, bool is_byte);

#ifdef __cplusplus
}
#endif

#endif // BYTESIZE_H
