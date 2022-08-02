# Bytesize

Bytesize is a C program that converts between byte representations.
Bytesize can convert between S.I and binary bytes (MB-MiB), between
binary-binary or si-si bytes (GiB-MiB, GB-MB), between vary large bytes
(100,000,000,000,000,000,000,000 B-PB), or convert inputs into
appropriate byte representations automatically.

## Building

### Bin

To build the executable:

``` bash
make release bin
```

This will generate the binary `build/release/bin/bytesize`.

### Lib

To build the library:
``` bash
make release lib
```

This will generate the dynamic library `build/release/lib/libbytesize.so`.

## Install

Run `sudo make install` to install the program to `/usr/local/bin/`.
If you want to change installation directory, run `PREFIX=/usr/bin/ make install`.

To uninstall `bytesize` run `sudo make uninstall`.

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

For more information see `bytesize.h`.

### Potential Features

- Simplify the `match()` function by returning a non-malloc'd copy of the string.
