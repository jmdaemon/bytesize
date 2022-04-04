#pragma once
#ifndef BYTESIZE_CLI_H
#define BYTESIZE_CLI_H

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pcre.h>
#include <math.h>

static const char *version = "v0.2.1";
static const char *email   = "<josephm.diza@gmail.com>";

/* Program documentation. */
static char doc[] = "Bytesize - Convert between byte representations";

/* A description of the arguments we accept. */
static char args_doc[] = "[from] [to]";

/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  {"units",  'u', 0,      0,  "Do not display byte units" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *args[2]; /* from & to */
  bool display_units;
  int verbose;
};

/* Parse a single option. */
error_t parse_opt (int key, char *arg, struct argp_state *state);

#endif // BYTESIZE_CLI_H
