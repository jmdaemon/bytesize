# Bytesize

Bytesize is a C program that converts between byte representations.
Bytesize can convert between S.I and binary bytes (MB-MiB), between
binary-binary or si-si bytes (GiB-MiB, GB-MB), between vary large bytes
(100,000,000,000,000,000,000,000 B-PB), or convert inputs into
appropriate byte representations automatically.

## Building

### Bin

To build the executable with make:

``` bash
make release bin
```

This will generate the binary `build/release/bin/bytesize`.


### Lib

To build the library with make:
``` bash
make release lib
```

This will generate the dynamic library `build/release/lib/libbytesize.so`.

### CMake

To build with CMake instead:

``` bash
cmake --preset gcc-release-unix-makefiles
cd build/gcc-release-unix-makefiles
make
```

This will build the `bytesize` executable, and shared and static libraries.

## Install

### Make
Run `sudo make install` to install the program to `/usr/local/bin/`.
If you want to change installation directory, run `PREFIX=/usr/bin/ make install`.

To uninstall `bytesize` run `sudo make uninstall`.

### CMake

To uninstall with CMake:

``` bash
cd build/gcc-release-unix-makefiles
cmake uninstall .
sudo make uninstall
```

Note that you must have first installed Bytesize(`install_manifest.txt`
has to be generated) in order for the uninstall to work.

## Usage

``` bash
# Handles converting between units 
$ bytesize 5GiB MB
5368.71 MB

# and automatically drops unnecessary decimals
$ bytesize 5GiB MiB
5120 MiB

# Can convert between between SI and Binary units 
$ bytesize 5MiB KB
5242.88 KB

# Can convert between large sizes manually
$ bytesize 100000000000000000000000B PB
1e+08 PB

# or automatically
$ bytesize 100000000000000000000000B
100 ZB

# Handle decimal inputs
$ bytesize 0.005GiB MiB
5.12 MiB
```

For more information see `bytesize --help`.

### Using as a Library

Bytesize provides various useful functions and facilities for working with bytes, converting between them, etc.

Some things to keep in mind when using `bytesize` as a library.

- Bytesize uses the `pcre`, `gmp`, and `mpfr` libraries, and must link with flags, `-lgmp`, `-lmpfr`, (and also `-lpcre` if you use the `match()` function), when linking.
- If you call `auto_size()` or `convert_units()` it is your responsibility to ensure that:
    - All matched input substrings are freed with `ppcre_free_substring(...)` (only when using `match()`)
    - All MPFR variables are freed with `mpfr_clear(...)`, and that memory is
        freed with `mpfr_free_cache2(MPFR_FREE_LOCAL_CACHE)` or `mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE)`
- If you are using the parsed output of `bytesize` keep in mind that
    - `display_units()` will return scientific numbers (e.g `1.0842e+09 YB`).

#### Examples

Here's an example that makes use of `convert_units()`:

``` c
#include <bytesize.h>

const char* input   = "5MiB";
const char* to      = "MB";
const char* digits  = get_amt(input);
const char* from    = get_unit(input);

Byte to = convert_units(digits, from, to);

/* Print unit conversion */
display_units(to.amt, to, true);

/* Deallocate */
pcre_free_substring(digits);
pcre_free_substring(from);

mpfr_clear(to.amt);
mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
```

Here is an example that makes use of `auto_size()`:

``` c
#include <bytesize.h>

const char* input   = "5MiB";
const char* to      = "MB";
const char* digits  = get_amt(input);
const char* from    = get_unit(input);
int scale           = 1;

/* Dynamically sets the scale depending on the input unit given.
   Defaults to SI units if bytes are given. */
if (!is_byte(from))
  scale = (found_in(from, SI_BYTE, SIZE)) ? SI_SCALE : BINARY_SCALE;

/* Initialize and set the amount to convert */
mpfr_t amt;
mpfr_inits2(200, amt, NULL);
mpfr_init_set_str(amt, digits, 10, MPFR_RNDF);

/* If the input is in bytes, the scale will be be 1 (B-B).
  If the input is not in bytes, then a relative scaling (xB->B) must be provided */
Byte to = (is_byte(from)) ?
  auto_size(amt, scale, true) : auto_size(amt, get_factor(units_from), false);

/* Print unit conversion */
display_units(to.amt, to, true);

/* Deallocate */
pcre_free_substring(digits);
pcre_free_substring(from);

mpfr_clear(to.amt);
mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE);
```

For more information see `bytesize.h`, `main.c` and the unit tests in `test/test_bytesize.c`.


### Docs

To view the documentation for `bytesize`:

``` bash
doxygen
```

The resulting HTML documentation will be found under `build/docs/html/`

### Potential Features

- Simplify the `match()` function by returning a non-malloc'd copy of the string.
- Passing all `lintian` checks on the `CPack` generated Debian packages.
- Create windows installers that install the binary and package to its own specific directory.
- Generate new releases
    - Automatic GitHub action that creates releases
    - Create release more manually with git hooks & bash aliases
