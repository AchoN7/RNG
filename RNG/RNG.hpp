#pragma once

/*	https://github.com/Achozzi/RNG
	A simple random number generator class with an easy to use interface.
	It works based on the Mersenne twister for efficiency.

	The generator is templated, so that whatever range you pass
		to the constructor, whatever the type (integral or decimal types, but not char types),
		it will automatically set it up for you.

	It's a lightweight class, so you can use it on the stack.
		Also compatible with smart pointers.
		Also thread safe.
		Also parallel. (now 16x faster as per tests)
		Also supports default bounds based on the supplied <type> through the constructor.

	********** Example usage:
	RNG<int> rng(-100, 100); --create a generator of int values
	int new_value = rng.gen_value();      --returns an int value
	It is as simple as that!

	You can create an instance as above and send a reference to it
		to as many threads as you like. Call rng.gen_value() within them,
		the threads can get values in parallel, without locking each other out.

	********** Additional functionality:
	If no bounds are provided, RNG<type> will automatically
		set the bounds as the min/max values of the provided <type>!
		RNG<short> rng{}; -- bound is set to (-32768; 32767)

	rng.change_bounds(new_lower_bound, new_upper_bound);	-- sets a new bound for the distribution
	rng.reseed();		-- sets a new seed for the generator to use

	********** BONUS!
	It can even be used with bools!
	RNG<bool> rng(false, true); --only use false and true
	bool new_value = rng.gen_value(); -- returns either 1 or 0.

	********** IMPORTANT!
	If RNG<type> is instantiated with bounds values that exceed the <type>'s supported values
		{e.g. RNG<short> rng(-1'000'000, 1'000'000), when short supports min:-32768 and max:32767}
		then the bounds will automatically be clamped to facilitate the <short>'s min/max bounds!

*/

#include <concepts>
#include <random>
#include <mutex>
#include <type_traits>
#include <limits>
#include <algorithm>

using std::same_as;
using std::is_same_v;
using std::is_integral_v;
using std::is_floating_point_v;
using std::numeric_limits;

using std::uniform_real_distribution;
using std::uniform_int_distribution;
using std::bernoulli_distribution;

using std::conditional_t;
using std::mutex;
using std::lock_guard;

template<typename T>
concept basic_types =
is_integral_v<T> ||
is_floating_point_v<T> ||
same_as<T, bool>;

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

	RNG() :
		seed(std::random_device()()),
		lower_bound(numeric_limits<T>::min()),
		upper_bound(numeric_limits<T>::max()),
		distribution(make_distribution(lower_bound, upper_bound)) {}

	RNG(T from, T to) :
		seed(std::random_device()())
	{
		bounds_check(from, to);
		distribution = make_distribution(lower_bound, upper_bound);
	}

	inline T gen_value() {
		static thread_local std::mt19937 generator(seed);
		return distribution(generator);
	}

	void change_bounds(T from, T to) {
		lock_guard<mutex> lock(mtx);
		bounds_check(from, to);
		distribution = make_distribution(lower_bound, upper_bound);
	}

	void reseed() {
		lock_guard<mutex> lock(mtx);
		seed = std::random_device()();
	}

	inline T get_lower_bound() {
		lock_guard<mutex> lock(mtx);
		return lower_bound;
	}

	inline T get_upper_bound() {
		lock_guard<mutex> lock(mtx);
		return upper_bound;
	}

	static void* operator new(size_t sz) {
		return ::operator new(sz);
	}

	static void operator delete(void* ptr) {
		::operator delete(ptr);
	}

	RNG(const RNG&) = delete;
	RNG(RNG&&) = delete;
	RNG& operator=(RNG&&) = delete;
	RNG& operator=(const RNG&) = delete;

private:

	unsigned int seed;
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

	void bounds_check(T from, T to) {
		//set the lower and upper values from the params in the appropriate order, just a precaution
		lower_bound = std::min(from, to);
		upper_bound = std::max(from, to);

		//runtime clamp of bounds if the provided range exceeds the range of the <type>
		lower_bound = std::clamp(lower_bound, numeric_limits<T>::min(), numeric_limits<T>::max());
		upper_bound = std::clamp(upper_bound, numeric_limits<T>::min(), numeric_limits<T>::max());
	}
};