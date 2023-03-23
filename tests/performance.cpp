#include "pch.h"
#include "CppUnitTest.h"

#include "RNG.hpp"
#include "RNG_Thread_Local.hpp"
#include "RNGtoString.hpp"

#include <thread>
#include <vector>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace performance {

	using namespace std;

	TEST_CLASS(RNG_Performance) {
	public:

		//TEST_METHOD(Test_LOCKING_performance) { //expected to be poor
		//	int thread_count = 12;
		//	int random_numbers = 10000;
		//	auto start = std::chrono::high_resolution_clock::now();

		//	RNG<int> rng(0, 100);

		//	vector<jthread> threads;
		//	for (int i = 0; i < thread_count; i++) {
		//		threads.emplace_back([&rng, &random_numbers]() {
		//			for (int j = 0; j < random_numbers; ++j) {
		//				int val = rng.gen_value();
		//			}
		//		});
		//	}

		//	for (auto& t : threads) {
		//		t.join();
		//	}

		//	auto end = std::chrono::high_resolution_clock::now();
		//	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

		//	string s{ to_string(thread_count) +
		//		" threads generated " + 
		//		to_string(random_numbers) + 
		//		" numbers in " + to_string(duration.count()) 
		//		+ " microseconds, each locking the generator." };

		//	Logger::WriteMessage(s.c_str());
		//}

		TEST_METHOD(Test_THREAD_LOCAL_performance) { //expected to be faster than the LOCKING version
			int thread_count = 12;
			int random_numbers = 10000;
			auto start = std::chrono::high_resolution_clock::now();

			RNG_Thread_Local<int> rng(0, 100);

			vector<thread> threads;
			for (int i = 0; i < thread_count; i++) {
				threads.emplace_back([&rng, &random_numbers]() {
					for (int j = 0; j < random_numbers; ++j) {
						int val = rng.gen_value();
					}
					});
			}

			for (auto& t : threads) {
				t.join();
			}

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

			string s{ to_string(thread_count) +
				" threads generated " +
				to_string(random_numbers) +
				" numbers in " + to_string(duration.count())
				+ " microseconds, each using its own generator." };

			Logger::WriteMessage(s.c_str());
		}
	};
}