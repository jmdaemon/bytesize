#include "cli.h"
#include "bytesize.h"

/* 
 * Calculates the conversion factor between a unit to another unit
 * given that they are both in either SI_BYTE or the BYTE  arrays.
 * Note that this also means that this does not convert between SI_BYTE to BYTE
 */
long int calc_factor(const char *unit, int size, const char *BYTE_FORMAT[], int scale) {
  for (int i = 0; i < size; i++) {
    const long int factor = pow(scale, i + 1);
    if (strcmp(unit, BYTE_FORMAT[i]) == 0)
      return factor;
  }
  return 0;
}

bool found_in(const char *elem, const char *array[], int array_size) {
  for (int i = 0; i < array_size; i++)
    if (strcmp(elem, array[i]) == 0)
      return true;
  return false;
}

long int get_factor(const char *unit) {
  int scale = (found_in(unit, SI_BYTE, SIZE)) ? (int) pow(10, 3) : (int) pow(2, 10);
  return (strlen(unit) == 2) ? calc_factor(unit, SIZE, SI_BYTE, scale) : calc_factor(unit, SIZE, BYTE, scale);
}

const char *match(char *input, const char *regex) {
  /* for pcre_compile */
  pcre *re;
  const char *error;
  int erroffset;

  /* for pcre_exec */
  int rc;
  int ovector[32];

  /* to get substrings from regex */
  const char *substring;

  /* we'll start after the first quote and chop off the end quote */
  const char *subject = input;

  re = pcre_compile(regex, 0, &error, &erroffset, NULL);
  rc = pcre_exec(re, NULL, subject, strlen(subject), 0, 0, ovector, 30);

  if (rc == PCRE_ERROR_NOMATCH) {
    fprintf(stderr,"no match\n");
    exit(0);
  } else if (rc < -1) {
    fprintf(stderr,"error %d from regex\n",rc);
    exit(rc);
  }

  pcre_get_substring(subject, ovector, rc, 0, &substring);
  pcre_free(re);

  return substring;
}

struct arguments set_default_args() {
  struct arguments arguments;

  /* Default values. */
  arguments.verbose = 0;
  arguments.display_units = true;
  return arguments;
}

void display_units(const double conversion, const char* units, bool show_with_units) {
  if (ceil(conversion) == (int) conversion) 
    (show_with_units) ? printf("%d %s\n", (int) conversion, units) : printf("%d\n", (int) conversion);
  else
    (show_with_units) ? printf("%.2f %s\n", conversion, units) : printf("%.2f\n", conversion);
}
const char* get_unit(char *input) {
  const char *units = match(input, unit_regex);
  return units;
}

double convert_units(char* input, const char* units_from, const char* units_to, int verbose) {
  long int from  = get_factor(units_from);
  long int to    = get_factor(units_to);

  const int amt = atoi(match(input, num_regex));
  if (verbose == 1) 
    printf("Amount To Convert: %d\n", amt);

  if (verbose == 1) {
    printf("Conversion Factor From: %ld\n", from);
    printf("Conversion Factor To: %ld\n", to);
  }

  const float factor = (float) from / to;
  const double conversion = amt * factor;
  return conversion;
}

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
