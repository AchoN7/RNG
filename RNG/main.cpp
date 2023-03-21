#include "Defines.hpp"
#include "RNG.hpp"

#include <iostream>

u32 main(u32 argc, char* argv[]) {

	RNG<i32> rng(-400, 400);
	i32 value = rng.gen_value();

	cout << "Randomly generated number between -400 and 400 is: " << value;

	return EXIT_SUCCESS;
}