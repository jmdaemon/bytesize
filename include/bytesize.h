#pragma once
#ifndef BYTESIZE_H
#define BYTESIZE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pcre.h>
#include <math.h>

#define SIZE 4

static const char *unit_regex = "([KMGT]i?B+)";
static const char *num_regex = "([\\d]+)";

static const char *SI_BYTE[SIZE] = {
  "KB",
  "MB",
  "GB",
  "TB"
};

static const char *BYTE[SIZE] = {
  "KiB",
  "MiB",
  "GiB",
  "TiB"
};

long int calc_factor(const char *unit, int size, const char *BYTE_FORMAT[], int scale);

bool found_in(const char *elem, const char *array[], int array_size);

long int get_factor(const char *unit);

const char *match(char *input, const char *regex);

#endif // BYTESIZE_H