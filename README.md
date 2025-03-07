# Heapless C++ String Library

`lstring` handles common string operations without heap allocation, allowing small targets to not run out of RAM.

```C++
char buf[24];
int num = 123;

lstring str(buf, sizeof(buf));
str.printf("%d", num).cat(" crows in the sky");
str.printf("%d", num).cat(" cows in the meadow, but this too long message won't break anything (and will finish by char 0)");
```

`lstring`
* stores the length and capacity, with the string data being an external buffer provided by the user,
* prevents memory fragmentation leading to memory loss,
* is adapted to small targets badly or not managing dynamic memory allocation (baremetal, Arduino, FreeRTOS, Mbed OS, RIOT OS).
* provides standard functionalities like concatenating, printf-like formatting, trimming,
* lightly uses lib C strings function, in particular not using heavy C++ streams,
* always checks string size to avoid buffer overruns,
* allows you to easily chain operations on strings.

Although this library uses standard C size-limited string functions brought by `<string.h>`, it does not use C11 functions (safe string functions with suffix `_s`) as it is not a part of gcc or clang standard lib C. Using [Intel safestringlib](https://github.com/intel/safestringlib), bringing these functions against common strings vulnerabilities is an option for the future, although increasing the footprint of this library.

## Why

This is from real life.

* `std::string` generally uses heap allocation when the string size exceeds the short string optimization (SSO) limit.
* Embedded libraries like Arduino's define convenient string classes like `String` using heap allocation when modifying strings.

Using these objects can lead to many short allocations, that can dramatically shrink the available RAM.

## Principles

* `lstring` members
    * `str`: pointer on the string, always provided by the user at creation time, except in functions `allocateCopy` and `allocateEmpty`
    * `cap`: maximum size of the string, including final `\0`
    * `len`: current size of the string excluding final `\0`, equivalent to safe `strnlen(str, capacity)`

* Its methods needing a single string pointer for convenience always use array references, never raw pointers. These methods are declared as templates.
* `lstring` code is in a header file, in order to help with templates compilation.
* `lstring` methods return object reference, allowing to chain operations.
* `lstring` methods check the size of strings to avoid buffer overruns.

## Examples

Please refer to `lstring.h` for examples.
