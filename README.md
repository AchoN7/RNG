# RNG (requires C++20)
### My own Random Number Generator template class with a simple interface that supports integer, decimal and bool types.
### Is thread safe and parallel-capable.

Lightweight, gets the job done. When instantiating, just supply the type, and/or the range that you want a random number from.
If you exceed the range supported by the provided type, then it will automatically be clamped to accommodate it.

### supports:
* most integral types (from std::is_integral_v)
* most decimal types (from std::is_floating_point_v)
* AND bool!

### Example with an integer type:
``` cpp
RNG<int> rng(-400, 400);
int value = rng.gen_value();
```

### Example with bools:
``` cpp
RNG<bool> rng(false, true);
bool value = rng.gen_value(); //returns 0 or 1
```

### Example without providing bounds:
``` cpp
RNG<short> rng{}; //bounds are automatically -32768 to 32767
short value = rng.gen_value(); //returns between -32768 and 32767 inclusive
```

### Compiled with MSVC and tested on VisualStudio 22 with CppUnitTests
