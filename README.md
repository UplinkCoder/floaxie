floaxie
=======
[![Build Status in GCC/Clang](https://travis-ci.org/aclex/floaxie.svg?branch=master)](https://travis-ci.org/aclex/floaxie) [![Build status in Visual Studio](https://ci.appveyor.com/api/projects/status/nhidn1n2o66etirk?svg=true)](https://ci.appveyor.com/project/aclex/floaxie) [![Code coverage](https://codecov.io/gh/aclex/floaxie/branch/master/graph/badge.svg)](https://codecov.io/gh/aclex/floaxie)

Floaxie is C++14 header-only library for printing floating point values of arbitrary precision (`float`, `double` etc.) and parsing their textual representation back again (in ordinary or exponent notation).

What is it for?
---------------

One is obviously not worried too much about the speed of the values being printed on the console, so the primary places to use the library are readers, writers, encoders and decoders of different text-based formats (e.g. JSON, XML and so on), applications interacting through text pipes or rendering data structures.

Compiler compatibility
----------------------
- [x] GCC 5
- [x] Clang 3.6
- [x] Visual Studio 2017

Printing
--------
**Grisu2** algorithm is adopted for printing floating point values. It is fast printing algorithm described by Florian Loitsch in his [Printing Floating-Point Numbers Quickly and Accurately with Integers](http://florian.loitsch.com/publications/dtoa-pldi2010.pdf) paper. **Grisu2** is chosen as probably the fastest **grisu** version, for cases, where shortest possible representation in 100% of cases is not ultimately important. Still it guarantees the best possible efficiency of more, than 99%.

Parsing
-------
The opposite to **Grisu** algorithm used for printing, an algorithm on the same theoretical base, but for parsing, is developed. Following the analogue of **Grisu** naming, who essentially appears to be cartoon character (Dragon), parsing algorithm is named after another animated character, rabbit, **Krosh:** ![Krosh](http://img4.wikia.nocookie.net/__cb20130427170555/smesharikiarhives/ru/images/0/03/%D0%9A%D1%80%D0%BE%D1%88.png "Krosh")

The algorithm parses decimal mantissa to extent of slightly more decimal digit capacity of floating point types, chooses a pre-calculated decimal power and then multiplies the two. Since the [rounding problem](http://www.exploringbinary.com/decimal-to-floating-point-needs-arbitrary-precision/) is not uncommon during such operations, and, in contrast to printing problem, one can't just return incorrectly rounded parsing results, such cases are detected instead and slower, but accurate fallback conversion is performed (C Standard Library functions like `strtod()` by default). In this respect **Krosh** is closer to **Grisu3**.

Example
-------
**Printing:**
```{.cpp}
#include <iostream>

#include "floaxie/ftoa.h"

using namespace std;
using namespace floaxie;

int main(int, char**)
{
	double pi = 0.1;
	char buffer[128];

	ftoa(pi, buffer);
	cout << "pi: " << pi << ", buffer: " << buffer << endl;

	return 0;
}
```

**Parsing:**
```{.cpp}
#include <iostream>

#include "floaxie/atof.h"

using namespace std;
using namespace floaxie;

int main(int, char**)
{
	char str[] = "0.1";
	char* str_end;
	double pi = 0;

	pi = atof<double>(str, &str_end);
	cout << "pi: " << pi << ", str: " << str << ", str_end: " << str_end << endl;

	return 0;
}
```
