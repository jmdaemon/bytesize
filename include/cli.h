#pragma once
#ifndef BYTESIZE_CLI_H
#define BYTESIZE_CLI_H

/* Single source program version */
#include "version.h"

/* Support header include for C++ */
#ifdef __cplusplus
extern "C" {
#endif

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pcre.h>
#include <math.h>

/** Author and maintainer's email. Contact me directly here for questions or bug reports, or use the issues page. */
static const char *email = "<josephm.diza@gmail.com>";

/** Program summary. */
static const char doc[] = "Bytesize - Convert between byte representations";

/** A description of the arguments we accept. */
static const char args_doc[] = "[from] [to]";

/** http://www.gnu.org/software/libc/manual/html_node/Argp-Option-Vectors.html <br> */
/** The options we understand. */
static const struct argp_option options[] = {
  {"si"     ,  500, 0,      0, "Uses SI byte scaling in autosize mode", 0},
  {"bi"     ,  600, 0,      0, "Uses binary byte scaling in autosize mode", 0},
  {"verbose",  'v', 0,      0, "Produce verbose output", 0},
  {"version",  'V', 0,      0, "Show program version", 0},
  {"units"  ,  'u', 0,      0, "Do not display byte units", 0},
  { 0 }
};

/** Used by main to communicate with parse_opt. */
struct arguments {
  char *args[2]; /** from, to */
  bool display_units;
  int verbose;
  int scale;
};

/** Parse a single option. */
error_t parse_opt (int key, char *arg, struct argp_state *state);

/** http://www.gnu.org/software/libc/manual/html_node/Argp-Parsers.html <br> */
/** http://www.gnu.org/software/libc/manual/html_node/Argp-Help-Filtering.html <br> */
/** Our argp parser. */
static const struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0};

/** Set default values for arguments */
struct arguments set_default_args();

#ifdef __cplusplus
}
#endif

#endif // BYTESIZE_CLI_H
