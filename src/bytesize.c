#include "bytesize.h"

/* Determines if a string is equal to some pattern */
bool smatch(const char* input, const char* pattern) {
  bool is_equal = (strcmp(input, pattern) == 0) ? true : false;
  return is_equal;
}

/* Determine if a specified unit is a byte */
bool is_byte(const char* unit) {
  return smatch(unit, "B");
}

/* 
 * Calculates the conversion factor between a unit to another unit
 * given that they are both in either SI_BYTE or the BYTE  arrays.
 * Note that this also means that this does not convert between SI_BYTE to BYTE
 */
long int calc_factor(const char *unit, int size, const char *BYTE_FORMAT[], int scale) {
  if (is_byte(unit))
    return 1;
  for (int i = 0; i < size; i++) {
    const long int factor = pow(scale, i + 1);
    if (smatch(unit, BYTE_FORMAT[i]))
      return factor;
  }
  return 0;
}

/* Determine if an element is found in the character array */
bool found_in(const char *elem, const char *array[], int array_size) {
  for (int i = 0; i < array_size; i++)
    if (smatch(elem, array[i]))
      return true;
  return false;
}

/* Determine the relative scaling of a unit with respect to a binary or si byte */
long int get_factor(const char *unit) {
  int scale = (found_in(unit, SI_BYTE, SIZE)) ? SI_SCALE : BINARY_SCALE;
  return (strlen(unit) == 2) ? calc_factor(unit, SIZE, SI_BYTE, scale) : calc_factor(unit, SIZE, BYTE, scale);
}

/* Determine if there are any regex matches with the input text */
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
    fprintf(stderr, "no match\n");
    exit(0);
  } else if (rc < -1) {
    fprintf(stderr, "error %d from regex\n",rc);
    exit(rc);
  }

  pcre_get_substring(subject, ovector, rc, 0, &substring);
  pcre_free(re);

  return substring;
}

/* Formats and displays the converted size */
void display_units(const double conversion, const char* units, bool show_with_units) {
  if (ceil(conversion) == (int) conversion) 
    (show_with_units) ? printf("%d %s\n", (int) conversion, units) : printf("%d\n", (int) conversion);
  else
    (show_with_units) ? printf("%.2f %s\n", conversion, units) : printf("%.2f\n", conversion);
}

/* Returns the byte size unit */
const char* get_unit(char *input) {
  const char *units = match(input, unit_regex);
  return units;
}

/* Converts an integral number between byte sizes */
double convert_units(char* input, const char* units_from, const char* units_to) {
  const bool is_from_bytes = is_byte(units_from);
  const bool is_to_bytes = is_byte(units_to);

  long int from = 1;
  long int to = 1;
  if (is_from_bytes && is_to_bytes) {
    // Bytes to Bytes
    from = 1;
    to = 1;
  } else if (is_from_bytes) {
    // Bytes to anything
    from = 1;
    to = get_factor(units_to);
  } else if (is_to_bytes) {
    // Anything to Bytes
    from  = get_factor(units_from);
    to = 1;
  } else {
    from  = get_factor(units_from);
    to    = get_factor(units_to);
  }

  const int amt = atoi(match(input, num_regex));
  log_debug("Amount To Convert      : %d", amt);
  log_debug("Conversion Factor From : %ld", from);
  log_debug("Conversion Factor To   : %ld", to);

  const double factor = (double) from / to;
  const double conversion = amt * factor;
  return conversion;
}

/* Automatically determines the best size to use
   and returns the converted unit */
Byte auto_size(size_t bytes, size_t scale) {
  const int i = floor(log(bytes) / log(scale)); 
  double amt = (bytes / pow(scale, i)); 
  const char* unit = (scale == SI_SCALE) ? SI_BYTE[i]: BYTE[i];
  Byte byte = {amt, unit};
  return byte;
}
