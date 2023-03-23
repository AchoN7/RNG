# RNG (requires C++20)
### My own Random Number Generator template class with a simple interface that supports integer, decimal and bool types.

Lightweight, gets the job done. When instantiating, just supply the type, and the range that you want a random number from.
Be careful not to exceed the ranges that the types support!

### supports:
* short/ushort
* int/uint
* long/ulong
* float/double
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
### Compiled with MSVC and tested on VisualStudio 22
