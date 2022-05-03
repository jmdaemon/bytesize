#include "cli.h"
#include "bytesize.h"

int main (int argc, char **argv) {
  log_set_level(LOG_ERROR);
  struct arguments arguments = set_default_args();
  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  int verbose = arguments.verbose;

  // Set log level
  if (verbose == 1)
    log_set_level(LOG_TRACE);
  else
    log_set_level(LOG_ERROR);

  log_info("Arguments: ");
  log_info("Args[0]: %s", arguments.args[0]);
  log_info("Args[1]: %s", arguments.args[1]);
  log_info("Verbose: %s", (verbose == 0) ? "Off" : "On");
  log_info("Display with units: %s", (arguments.display_units) ? "True" : "False");

  const char *units_from = get_unit(arguments.args[0]);
  const char *units_to   = get_unit(arguments.args[1]);
  log_info("Units From: %s", units_from);
  log_info("Units To: %s", units_to);

  const double conversion = convert_units(arguments.args[0], units_from, units_to);

  display_units(conversion, units_to, arguments.display_units);

  pcre_free_substring(units_from);
  pcre_free_substring(units_to);
  return 0;
}
