#pragma once

/*	https://github.com/Achozzi/RNG
	A simple random number generator class with an easy to use interface.
	It works based on the Mersenne twister for efficiency.

	The generator is templated, so that whatever range you pass 
		to the constructor, whatever the type (integer or decimal types),
		it will automatically set it up for you.

	It's a lightweight class, so you can use it on the stack.
		Also compatible with smart pointers.
		Also thread safe.
	
	**************
	Example usage:
	Random_Generator<i16> rng(-100, 100); --create a generator of i16 values
	i16 new_value = rng.gen_value();      --returns an i16 value
	It is as simple as that!

	*************************
	Additional functionality:
	rng.change_bounds(new_lower_bound, new_upper_bound);	-- sets a new bound for the distribution
	rng.reseed();		-- sets a new seed for the generator to use

	**************************************
	BONUS. It can even be used with bools!
	Random_Generator<bool> rng(false, true); --only use false and true
	bool new_value = rng.gen_value(); -- returns either 1 or 0.
*/

#include <random>
#include <mutex>
#include <type_traits>

#include "Defines.hpp"

template <typename T>
concept basic_types = 
use<T, i16 > OR
use<T, u16 > OR 
use<T, i32 > OR 
use<T, u32 > OR
use<T, i64 > OR 
use<T, u64 > OR 
use<T, f32 > OR
use<T, f64 > OR 
is_same_v<T, bool>;

template <basic_types T>
class RNG {

public:

	/*  If T is a decimal, it will use a URD, else if it's a boolean
		it will use BD, and finally if integer type UID. */
	using Distribution = conditional_t<
		is_floating_point_v<T>,
		uniform_real_distribution<T>,
		conditional_t<
			is_same_v<T, bool>,
			bernoulli_distribution,
			uniform_int_distribution<T>
		>
	>;

	RNG<T>(T from, T to) :
		device(),
		generator(device()),
		distribution(make_distribution(from, to)),
		lower_bound(from), upper_bound(to) {}

	T gen_value() { 
		lock_guard<mutex> lock(mtx);
		return distribution(generator);
	}

	void change_bounds(T from, T to) {
		lock_guard<mutex> lock(mtx);
		lower_bound = from;
		upper_bound = to;

		distribution = make_distribution(from, to);
	}

	void reseed() { 
		lock_guard<mutex> lock(mtx);
		generator.seed(random_device()()); 
	}

	T get_lower_bound() const { return lower_bound; }
	T get_upper_bound() const { return upper_bound; }

	static void* operator new(size_t sz) {
		return ::operator new(sz);
	}

	static void operator delete(void* ptr) {
		::operator delete(ptr);
	}

private:

	random_device device;
	mt19937 generator;
	Distribution distribution;

	T lower_bound;
	T upper_bound;

	mutex mtx;

	Distribution make_distribution(T from, T to) {
		if constexpr (is_same_v<T, bool>) {
			return bernoulli_distribution(0.5);
		}
		else {
			return Distribution(from, to);
		}
	}
};