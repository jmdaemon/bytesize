#include "cli.h"
#include "bytesize.h"

int main (int argc, char **argv) {
  log_set_level(LOG_ERROR);
  struct arguments arguments = set_default_args();
  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  int verbose = arguments.verbose;
  char* input = arguments.args[0];
  char* output = (strlen(arguments.args[1]) == 0) ? "Auto" : arguments.args[1];
  int scale = arguments.scale;

  /* Set log level */
  if (verbose == 1)
    log_set_level(LOG_TRACE);
  else
    log_set_level(LOG_ERROR);

  log_info("Arguments: ");
  log_info("Args[0]: %s", input);
  log_info("Args[1]: %s", output);
  log_info("Verbose: %s", (verbose == 0) ? "Off" : "On");
  log_info("Display with units: %s", (arguments.display_units) ? "True" : "False");
  log_info("Automatic Size Mode: %s", smatch(output, "Auto") ? "On" : "Off" );
  if (smatch(output, "Auto")) 
    log_info("Scale: %d", scale);

  char *units_from = get_unit(input);
  char *units_to = "";

  /* Dynamically sets the scale depending on the input unit given.
     Defaults to SI units if bytes are given. */
  if (!is_byte(units_from))
    scale = (found_in(units_from, SI_BYTE, SIZE)) ? SI_SCALE : BINARY_SCALE;

  if (!smatch(output, "Auto"))
    units_to   = get_unit(output);

  Byte to;
  mpfr_t amt; /* Note that to.amt will be created later on, so we should not allocate it here */
  const char* digits = get_amt(input);
  mpfr_init_set_str(amt, digits, 10, MPFR_RNDF); /* Initializes and sets amt in one line */

  if (smatch(output, "Auto")) 
    to = (is_byte(units_from)) ?
      auto_size(amt, scale, true) : auto_size(amt, get_factor(units_from), false);
  else 
    to = convert_units(digits, units_from, units_to);

  units_to = to.unit;

  display_units(to.amt, units_to, arguments.display_units);

  log_info("Units From: %s", units_from);
  log_info("Units To: %s", units_to);

  /* Deallocate */
  pcre_free_substring(digits);
  pcre_free_substring(units_from);

  if (!smatch(output, "Auto"))
    pcre_free_substring(units_to);

  if (!smatch(output, "Auto"))
    mpfr_clear(amt);
  mpfr_clear(to.amt) ;/* Deallocate the to.amt mpfr value here */
  mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
  return 0;
}
