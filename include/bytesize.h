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
#include "log.h"

#define SIZE 5

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
};

static const char *BYTE[SIZE] = {
  "KiB",
  "MiB",
  "GiB",
  "TiB",
  "PiB",
};

typedef struct Byte {
  int amt;
  char* unit;
} Byte;

typedef struct Conversion {
  Byte from;
  Byte to;
} Conversion;

bool smatch(const char* input, const char* pattern);
long int calc_factor(const char *unit, int size, const char *BYTE_FORMAT[], int scale);
bool found_in(const char *elem, const char *array[], int array_size);
long int get_factor(const char *unit);
const char *match(char *input, const char *regex);
void display_units(const double conversion, const char* units, bool show_with_units);
const char* get_unit(char *input);
double convert_units(char* input, const char* units_from, const char* units_to);
Byte auto_size(size_t bytes, size_t scale);

#ifdef __cplusplus
}
#endif

#endif // BYTESIZE_H
