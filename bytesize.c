#include <stdlib.h>
#include <argp.h>

const char *version = "v0.1.0";
const char *email   = "<josephm.diza@gmail.com>";

/* Program documentation. */
static char doc[] = "Bytesize - Convert between byte representations";

/* A description of the arguments we accept. */
static char args_doc[] = "[from] [to]";

/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *args[2]; /* from & to */
  int verbose;
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;
  switch (key) {
    case 'v':
      arguments->verbose = 1;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 2)
        /* Too many arguments. */
        argp_usage (state);
      arguments->args[state->arg_num] = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 2)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv) {
  struct arguments arguments;

  /* Default values. */
  arguments.verbose = 0;

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  /*int from  = arguments.args[0];*/
  /*int to    = arguments.args[0];*/
  
  printf ("from = %s\nto = %s\nVERBOSE = %s\n",
          arguments.args[0], arguments.args[1],
          arguments.verbose ? "yes" : "no");

  exit (0);
}
