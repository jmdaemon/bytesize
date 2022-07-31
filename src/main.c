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
  char *units_to;

  /* Dynamically sets the scale depending on the input unit given.
     Defaults to SI units if bytes are given. */
  if (!is_byte(units_from))
    scale = (found_in(units_from, SI_BYTE, SIZE)) ? SI_SCALE : BINARY_SCALE;

  if (!smatch(output, "Auto"))
    units_to   = get_unit(output);

  /*long double conversion = 0;*/
  /*mpz_t amt, result;*/
  mpfr_t amt, conversion;
  mpfr_init2 (amt, 200);
  mpfr_init2 (conversion, 200);

  Byte to;
  mpfr_init2 (to.amt, 200);
  if (smatch(output, "Auto")) {
    /*unsigned long long int amt = get_amt();*/
    /*mpz_init_set_str(amt, input, 10);*/
    /*printf("%llu\n", amt);*/
    /*mpfr_init_set_str(amt, input, 10, MPFR_RNDZ);*/

    /*mpfr_init_set_str(amt, input, 10, MPFR_RNDF);*/
    /*mpfr_init_set_str(amt, get_amt(input), 10, MPFR_RNDF);*/
    char* digits = match(input, num_regex);
    printf("Digits: %s\n", digits);
    mpfr_init_set_str(amt, digits, 10, MPFR_RNDF);

    if (is_byte(units_from)) {
      puts("Executing is_byte() branch");
      to = auto_size(amt, scale, true);
    }
    else 
      to = auto_size(amt, get_factor(units_from), false);

    /*conversion = to.amt;*/
    /*mpfr_set(conversion, *to.amt, MPFR_RNDZ);*/
    /*mpfr_set(conversion, to.amt, MPFR_RNDZ);*/
    mpfr_set(conversion, to.amt, MPFR_RNDF);
    units_to = to.unit;
    /*printf("%s\n", to.unit);*/
  }
  printf("%s\n", units_to);

  /*printf ("Sum is ");*/
  /*mpfr_out_str (stdout, 10, 0, s, MPFR_RNDD);*/
  /*putchar ('\n');*/
  mpfr_out_str (stdout, 10, 0, conversion, MPFR_RNDD);
  putchar ('\n');

  mpfr_t res;
  mpfr_init2 (res, 200);
  // display_units
  // ceil(conversion)
  mpfr_ceil(res, conversion);
  /*if (ceil(conversion) == (unsigned long long int) conversion) */
  if (mpfr_cmp (res, conversion) == 0)
    /*(arguments.display_units) ? mpfr_printf("%llu %s\n", (unsigned long long int) conversion, units_from) : printf("%llu\n", (unsigned long long int) conversion);*/
    /*(arguments.display_units) ? mpfr_printf("%Q %s\n", conversion, units_from) : mpfr_printf("%Q\n", conversion);*/
    /*(arguments.display_units) ? mpfr_printf("%llu %s\n", conversion, units_to) : mpfr_printf("%llu\n", conversion);*/
    /*(arguments.display_units) ? mpfr_printf("%Rf %s\n", conversion, units_to) : mpfr_printf("%Rf\n", conversion);*/
    /*(arguments.display_units) ? mpfr_printf("%Ff %s\n", conversion, units_to) : mpfr_printf("%Ff\n", conversion);*/
    /*(arguments.display_units) ? mpfr_printf("%Zd %s\n", conversion, units_to) : mpfr_printf("%Zd\n", conversion);*/

    /*(arguments.display_units) ? mpfr_printf("%Zu %s\n", conversion, units_to) : mpfr_printf("%Zu\n", conversion);*/
    (arguments.display_units) ? mpfr_printf("%Rf %s\n", conversion, units_to) : mpfr_printf("%Rf\n", conversion);

    /*(arguments.display_units) ? mpfr_printf("%Pd %s\n", conversion, units_from) : mpfr_printf("%Pd\n", conversion);*/
  else
    /*(arguments.display_units) ? printf("%.2Lf %s\n", conversion, units_from) : printf("%.2Lf\n", conversion);*/
    /*(arguments.display_units) ? mpfr_printf("%.2F %s\n", conversion, units_from) : mpfr_printf("%.2F\n", conversion);*/
    /*(arguments.display_units) ? mpfr_printf("%.2Lf %s\n", conversion, units_to) : mpfr_printf("%.2Lf\n", conversion);*/
    (arguments.display_units) ? mpfr_printf("%.2Rf %s\n", conversion, units_to) : mpfr_printf("%.2Rf\n", conversion);

  /*else {*/
    /*conversion = convert_units(input, units_from, units_to);*/
  /*}*/

  /*display_units(conversion, units_to, arguments.display_units);*/

  log_info("Units From: %s", units_from);
  log_info("Units To: %s", units_to);

  /* Deallocate */
  pcre_free_substring(units_from);

  if (!smatch(output, "Auto"))
    pcre_free_substring(units_to);

  /* Big numbers */
  /*mpfr_clear (*to.amt);*/
  mpfr_clear (amt);
  mpfr_clear (conversion);
  mpfr_clear (res);
  mpfr_free_cache();
  return 0;
}
