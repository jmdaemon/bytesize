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
# and automatically drops unnecessary decimals
# *Note* this is broken at the moment
$ bytesize 5MB GB
5120 MiB

# Can convert between between SI and Binary units 
# *Note* this is broken at the moment
$ bytesize 5MiB KB
5242.88 KB

# Can convert between large sizes manually
$ bytesize 100000000000000000000000B PB
100000000 PB

# or automatically
$ bytesize 100000000000000000000000B
100 ZB
```

For more information see `bytesize --help`.

### Using as a Library
Bytesize provides various useful functions and facilities for working with bytes, converting between them, etc.
Read `bytesize.h` for more information.
