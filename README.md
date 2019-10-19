# Format Float

Tool for dumping IEEE-754 floating point numbers in a variety to formats.
Supports half, single, and double precision float point numbers.

Author: Ewan Crawford <ewan.cr@gmail.com>

## Usage

```console
$ ff --help
Usage: ff [-f <float> | -h <half> | -d <double>]
Floats can be entered in hex integer, hex float, fixed point float, or scientific float formats
    -h, --half   <half>     16-bit half precision
    -f, --float  <float>    32-bit single precision
    -d, --double <double>   64-bit double precision
    --help                  Displays this message and exi
```

## TODO

* [ ] Testing
* [ ] Denormal halfs
* [ ] NAN, INF, 0
* [ ] RTE rounding

## License

This project is licensed under the MIT license.