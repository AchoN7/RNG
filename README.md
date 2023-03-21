# RNG
My own Random Number Generator template class with a simple interface that supports integer, decimal and bool types.

Lightweight, gets the job done. When instantiating, just supply the type, and the range that you want a random number from.
Be careful not to exceed the ranges that the types support!

supports:
short/ushort
int/uint
long/ulong
float/double
AND bool!

Example with an integer type:

``` cpp
RNG<i32> rng(-400, 400);
i32 value = rng.gen_value();
```
