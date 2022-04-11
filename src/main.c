#include "cli.h"
#include "bytesize.h"

int main (int argc, char **argv) {
  struct arguments arguments = set_default_args();
  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  const char *units_from = get_unit(arguments.args[0]);
  const char *units_to   = get_unit(arguments.args[1]);
  const double conversion = convert_units(arguments.args[0], units_from, units_to, arguments.verbose);

  display_units(conversion, units_to, arguments.display_units);

  pcre_free_substring(units_from);
  pcre_free_substring(units_to);
  return 0;
}
