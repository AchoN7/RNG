#include "RNG.hpp"

#include <iostream>

uint32_t main(uint32_t argc, char* argv[]) {

	RNG<int32_t> rng(-400, 400);
	int32_t value = rng.gen_value();

	std::cout << "Randomly generated number between -400 and 400 is: " << value;

	return EXIT_SUCCESS;
}