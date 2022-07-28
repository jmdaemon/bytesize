#include "cli.h"
#include "bytesize.h"

int main (int argc, char **argv) {
  log_set_level(LOG_ERROR);
  struct arguments arguments = set_default_args();
  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  int verbose = arguments.verbose;
  char* output = (strlen(arguments.args[1]) == 0) ? "Auto" : arguments.args[1];
  int scale = arguments.scale;

  /* Set log level */
  if (verbose == 1)
    log_set_level(LOG_TRACE);
  else
    log_set_level(LOG_ERROR);

  log_info("Arguments: ");
  log_info("Args[0]: %s", arguments.args[0]);
  log_info("Args[1]: %s", output);
  log_info("Verbose: %s", (verbose == 0) ? "Off" : "On");
  log_info("Display with units: %s", (arguments.display_units) ? "True" : "False");
  log_info("Automatic Size Mode: %s", smatch(output, "Auto") ? "On" : "Off" );
  if (smatch(output, "Auto")) 
    log_info("Scale: %d", scale);

  char *units_from = get_unit(arguments.args[0]);
  char *units_to;

  /* Dynamically sets the scale depending on the input unit given.
     Defaults to SI units if bytes are given. */
  if (!is_byte(units_from))
    scale = (found_in(units_from, SI_BYTE, SIZE)) ? SI_SCALE : BINARY_SCALE;

  if (!smatch(output, "Auto"))
    units_to   = get_unit(output);

  long double conversion = 0;
  if (smatch(output, "Auto")) {
    /*u_long amt = get_amt(arguments.args[0]);*/
    /*long long int amt = get_amt(arguments.args[0]);*/
    long int amt = get_amt(arguments.args[0]);
    /*printf("%Ld\n", amt);*/
    printf("%ld\n", amt);
    Byte to;
    if (is_byte(units_from))
      to = auto_size(amt, scale, true);
    else 
      to = auto_size(amt, get_factor(units_from), false);
    conversion = to.amt;
    units_to = to.unit;

    /*display_units(conversion, units_to, arguments.display_units);*/
    if (ceil(conversion) == (long long int) conversion) 
      /*(arguments.display_units) ? printf("%ld %s\n", (long) conversion, units_to) : printf("%ld\n", (long) conversion);*/
      (arguments.display_units) ? printf("%Ld %s\n", (long long int) conversion, units_to) : printf("%Ld\n", (long long int) conversion);
    else
      (arguments.display_units) ? printf("%.2Lf %s\n", conversion, units_to) : printf("%.2Lf\n", conversion);
      /*(arguments.display_units) ? printf("%.2f %s\n", conversion, units_to) : printf("%.2f\n", conversion);*/

  } else {
    conversion = convert_units(arguments.args[0], units_from, units_to);
    display_units(conversion, units_to, arguments.display_units);
  }

  log_info("Units From: %s", units_from);
  log_info("Units To: %s", units_to);


  pcre_free_substring(units_from);

  if (!smatch(output, "Auto"))
    pcre_free_substring(units_to);
  return 0;
}
