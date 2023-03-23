#include "pch.h"
#include "CppUnitTest.h"

#include "RNG.hpp"
#include "RNGtoString.hpp"

#include <thread>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests {

	using namespace std;

	TEST_CLASS(RandomNumberGenerator) {
	public:

		TEST_METHOD(Test_RNG_in_Range) {
			RNG<int> generator(0, 100);

			for (int i = 0; i < 100; i++) {
				int value = generator.gen_value();
				Assert::IsTrue(value >= 0 && value <= 100);
			}
		}

		TEST_METHOD(Test_function_change_bounds) {
			RNG<int> generator(0, 100);

			generator.change_bounds(50, 150);

			for (int i = 0; i < 100; i++) {
				int value = generator.gen_value();
				Assert::IsTrue(value >= 50 && value <= 150);
			}
		}
	
		TEST_METHOD(Test_function_reseed) {
			RNG<int> generator(0, 100);

			int value1 = generator.gen_value();

			generator.reseed();

			int value2 = generator.gen_value();

			Assert::AreNotEqual(value1, value2);
		}

		TEST_METHOD(Test_RNG_bools) {
			RNG<bool> generator(false, true);

			for (int i = 0; i < 100; i++) {
				bool value = generator.gen_value();
				Assert::IsTrue(value == true || value == false);
			}
		}

		TEST_METHOD(Test_RNG_floats) {
			RNG<float> generator(0.0f, 1.0f);

			for (int i = 0; i < 100; i++) {
				float value = generator.gen_value();
				Assert::IsTrue(value >= 0.0f && value <= 1.0f);
			}
		}

		TEST_METHOD(Test_RNG_doubles) {
			RNG<double> generator(-100.0, 100.0);

			for (int i = 0; i < 200; i++) {
				double value = generator.gen_value();
				Assert::IsTrue(value >= -100.0 && value <= 100.0);
			}
		}

		TEST_METHOD(Test_Smart_Pointer_Compatibility) {
			unique_ptr<RNG<int>> unique;
			{
				unique = make_unique<RNG<int>>(-100, 100);
				Assert::IsNotNull(unique.get());
			}
			unique_ptr<RNG<int>> new_unique(unique.release());
			Assert::IsNull(unique.get());
			Assert::IsNotNull(new_unique.get());
			

			shared_ptr<RNG<int>> shared1 = make_shared<RNG<int>>(-100, 100);
			shared_ptr<RNG<int>> shared2 = shared1;
			Assert::IsNotNull(shared1.get());
			Assert::AreEqual(shared1.get(), shared2.get());

			weak_ptr<RNG<int>> weak = shared1;
			Assert::IsFalse(weak.expired());
		}

		TEST_METHOD(Test_Concurrency_support) {
			RNG<int> rng(0, 100);

			vector<jthread> threads;
			for (int i = 0; i < 10; i++) {
				threads.emplace_back([&rng]() {
					for (int j = 0; j < 1000; j++) {
						int val = rng.gen_value();
						Assert::IsTrue(val >= 0 && val <= 100);
					}
				});
			}
		}

		TEST_METHOD(Test_Default_Constructor) {
			RNG<short> rng{};

			for (int i = 0; i < 100; i++) {
				char val = rng.gen_value();
				Assert::IsTrue(val >= -32767 && val <= 32768);
			}
		}
	};
}
