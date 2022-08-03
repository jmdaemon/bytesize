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
    case 'V':
      printf("bytesize %s v%d.%d.%d\n", email, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
      /*printf("%d\n", key);*/
      /*arguments->args[0] = NULL;*/
      /*arguments->args[1] = NULL;*/
      exit(0);
      /*return 2;*/

    case 'v':
      arguments->verbose = 1;
      break;

    case 'u':
      arguments->display_units = false;
      break;

    case 500:
      arguments->scale = 1000;
      break;

    case 600:
      arguments->scale = 1024;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num > 2)
        /* Too many arguments. */
        printf("Too many arguments. ");
        printf("%d\n", key);
        argp_usage (state);
      arguments->args[state->arg_num] = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 0) {
        printf("Not enough arguments. ");
        printf("%d\n", key);
        /* Not enough arguments. */
        argp_usage (state);
      }
      break;

    /*default:*/
      /*printf("Unknown argument. ");*/
      /*printf("%d\n", key);*/
      /*return ARGP_ERR_UNKNOWN;*/

    }
  return 0;
}
