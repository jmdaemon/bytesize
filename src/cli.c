#include "cli.h"

struct arguments set_default_args() {
  struct arguments arguments;

  /* Default values. */
  arguments.verbose = 0;
  arguments.display_units = true;
  arguments.scale = 1000;

  arguments.args[1] = "";
  return arguments;
}

error_t parse_opt (int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;
  switch (key) {
    case 'v':
      arguments->verbose = 1;
      break;

    case 'u':
      arguments->display_units = false;
      break;

    case 400:
      printf("bytesize %s %d.%d.%d", email, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
      break;

    case 500:
      arguments->scale = 1000;
      break;

    case 600:
      arguments->scale = 1024;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 2)
        /* Too many arguments. */
        argp_usage (state);
      arguments->args[state->arg_num] = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}
