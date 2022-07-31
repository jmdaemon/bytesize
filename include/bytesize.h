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

#define SIZE 8

static const int SI_SCALE = 1000;
static const int BINARY_SCALE = 1024;

static const char *unit_regex = "([KMGTP]?i?B+)";
static const char *num_regex = "([\\d]+)";

static const char *SI_BYTE[SIZE] = {
  "KB",
  "MB",
  "GB",
  "TB",
  "PB",
  "EB",
  "ZB",
  "YB"
};

static const char *BYTE[SIZE] = {
  "KiB",
  "MiB",
  "GiB",
  "TiB",
  "PiB",
  "EiB",
  "ZiB",
  "YiB"
};

typedef struct Byte {
  mpfr_t amt;
  char* unit;
  long int scaling;
} Byte;

typedef struct Scale {
  const int scale;
  const char** sizes;
} Scale;

static const struct Scale BINARY = { BINARY_SCALE, BYTE };
static const struct Scale SI = { SI_SCALE, SI_BYTE };

typedef struct Conversion {
  Byte from;
  Byte to;
} Conversion;

/* Helper Functions */
const char *match(char *input, const char *regex);
bool found_in(const char *elem, const char *array[], int array_size);
bool smatch(const char* input, const char* pattern);

/* Bytesize Functions */
bool is_byte(const char* unit);
Scale get_scale(const char *unit);
long int calc_factor(const char *unit, int size, const Scale scale);
long int get_factor(const char *unit);
void display_units(mpfr_t conversion, const char* units, bool show_with_units);
const char* get_unit(char *input);
const char* get_amt(char *input);
Byte convert_units(char* input, const char* units_from, const char* units_to);
Byte auto_size(mpfr_t bytes, int scale, bool is_byte);

#ifdef __cplusplus
}
#endif

#endif // BYTESIZE_H
