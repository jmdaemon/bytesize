#include "cli.h"

struct arguments set_default_args() {
  struct arguments arguments;

  /* Default values. */
  arguments.verbose = 0;
  arguments.display_units = true;
  return arguments;
}
