# Bytesize

Bytesize is a C program that converts between byte representations.
Bytesize can convert Megabytes to Mebibytes, and Gibibytes to Mebibytes.

## Building

Run `make release` to build `bytesize`.
This will generate a binary the `bytesize` binary in `build/release/bin/bytesize`.

## Installation

Run `sudo make install` to install the program to `/usr/local/bin/`.
If you want to change installation directory, run `PREFIX=/usr/bin/ make install`.

To uninstall `bytesize` run `sudo make uninstall`.

## Usage

``` bash
$ bytesize 5MB GB
5120.00 MiB
```
