#include "pch.h"
#include "CppUnitTest.h"

#include "RNG.hpp"
#include "Defines.hpp"
#include "RNGtoString.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests {

	TEST_CLASS(RandomNumberGenerator) {
	public:

		TEST_METHOD(Test_RNG_in_Range) {
			RNG<i32> generator(0, 100);

			for (i32 i = 0; i < 100; i++) {
				i32 value = generator.gen_value();
				Assert::IsTrue(value >= 0 AND value <= 100);
			}
		}

		TEST_METHOD(Test_function_change_bounds) {
			RNG<i32> generator(0, 100);

			generator.change_bounds(50, 150);

			for (i32 i = 0; i < 100; i++) {
				i32 value = generator.gen_value();
				Assert::IsTrue(value >= 50 AND value <= 150);
			}
		}
	
		TEST_METHOD(Test_function_reseed) {
			RNG<i32> generator(0, 100);

			i32 value1 = generator.gen_value();

			generator.reseed();

			i32 value2 = generator.gen_value();

			Assert::AreNotEqual(value1, value2);
		}

		TEST_METHOD(Test_RNG_bools) {
			RNG<bool> generator(false, true);

			for (i32 i = 0; i < 100; i++) {
				bool value = generator.gen_value();
				Assert::IsTrue(value == true OR value == false);
			}
		}

		TEST_METHOD(Test_RNG_floats) {
			RNG<f32> generator(0.0f, 1.0f);

			for (i32 i = 0; i < 100; i++) {
				f32 value = generator.gen_value();
				Assert::IsTrue(value >= 0.0f AND value <= 1.0f);
			}
		}

		TEST_METHOD(Test_RNG_doubles) {
			RNG<f64> generator(-100.0, 100.0);

			for (i32 i = 0; i < 200; i++) {
				f64 value = generator.gen_value();
				Assert::IsTrue(value >= -100.0 AND value <= 100.0);
			}
		}

		TEST_METHOD(Test_Smart_Pointer_Compatibility) {
			unique_ptr<RNG<i32>> unique;
			{
				unique = make_unique<RNG<i32>>(-100, 100);
				Assert::IsNotNull(unique.get());
			}
			unique_ptr<RNG<i32>> new_unique(unique.release());
			Assert::IsNull(unique.get());
			Assert::IsNotNull(new_unique.get());
			

			shared_ptr<RNG<i32>> shared1 = make_shared<RNG<i32>>(-100, 100);
			shared_ptr<RNG<i32>> shared2 = shared1;
			Assert::IsNotNull(shared1.get());
			Assert::AreEqual(shared1.get(), shared2.get());

			weak_ptr<RNG<i32>> weak = shared1;
			Assert::IsFalse(weak.expired());
		}
	
	};
}
