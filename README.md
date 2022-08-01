# Bytesize

Bytesize is a C program that converts between byte representations.
Bytesize can convert between S.I and binary bytes (MB-MiB), between
binary-binary or si-si bytes (GiB-MiB, GB-MB), between vary large bytes
(100,000,000,000,000,000,000,000 B-PB), or convert inputs into
appropriate byte representations automatically.

## Building

Run `make release bin` to build `bytesize` in release configuration.
This will generate a binary the `bytesize` binary in `build/release/bin`.

## Installation

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

- Bytesize uses the `pcre`, `gmp`, and `mpfr` libraries, and must link with flags, `-lpcre`, `-lgmp`, `-lmpfr` when compiling.
- If you call `auto_size()` or `convert_units()` it is your responsibility to ensure that:
    - All matched input substrings are freed with `ppcre_free_substring(...)` (`convert_units()` only)
    - All `mpfr` variables are freed with `mpfr_clear(...)`, and that the cache is
        freed with `mpfr_free_cache2(MPFR_FREE_LOCAL_CACHE)` or `mpfr_free_cache2(MPFR_FREE_GLOBAL_CACHE)`
- If you are using the parsed output of `bytesize` keep in mind that
    - `display_units()` will return scientific numbers such as `1.0842e+09 YB`.

For more information see `bytesize.h`.

### Potential Features

- Hiding the ppcre library from compilation by either:
    1. Including parameters directly in `convert_units()` to specify the matched substrings to.
    2. Returning a string copy in match, simplifying the overhead of remembering to deallocate properly.
