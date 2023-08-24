#include <gmpxx.h>
#include <iostream>
#include <ctime>
#include <thread>
#include <vector>
#include <fmt/format.h>

#ifndef EM_PORT_API
#	if defined(__EMSCRIPTEN__)
#		include <emscripten.h>
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#		else
#			define EM_PORT_API(rettype) rettype EMSCRIPTEN_KEEPALIVE
#		endif
#	else
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype
#		else
#			define EM_PORT_API(rettype) rettype
#		endif
#	endif
#endif

void Factorial(const mpz_class& start, const mpz_class& end, mpz_class* out)
{
	mpz_class result{ start };
	for (mpz_class i = start + 1; i <= end; i += 1)
	{
		result *= i;
	}
	*out = result;
}

EM_PORT_API(int) main(void)
{
	unsigned long number{ 5000 };
	fmt::println("0.Single Thread    1.Multi Threads");
	fmt::print("Thread Type (0/1): ");
	int thread_type{ 0 };
	std::cin >> thread_type;
	if (thread_type < 0 || thread_type > 1)
	{
		fmt::println("Error Input");
		system("pause");
		return 0;
	}
	fmt::print("Number: ");
	std::cin >> number;

	const clock_t start{ clock() };

	mpz_class result = 1;
	if (thread_type == 0)
	{
		// single thread
		Factorial(1, number, &result);
	}
	else if (thread_type == 1)
	{
		// multi thread
		const unsigned thread_count{ std::thread::hardware_concurrency() * 3 > number ? number : std::thread::hardware_concurrency() * 3 };
		fmt::println("Thread_Count: {}", thread_count);
		std::vector<std::thread> threads;
		std::vector<mpz_class> temp(thread_count, mpz_class());
		std::vector<mpz_class*> results(thread_count);
		std::ranges::generate(results.begin(), results.end(), []() { return new mpz_class(); });

		const unsigned long step{ number / thread_count - 1 };
		temp[0] = 1;
		threads.emplace_back(Factorial, temp[0], temp[0] + step, results[0]);
		for (unsigned i = 1; i < thread_count - 1; i++)
		{
			temp[i] = temp[i - 1] + step + 1;
			threads.emplace_back(Factorial, temp[i], temp[i] + step, results[i]);
		}
		threads.emplace_back(Factorial, temp[thread_count - 2] + step + 1, number, results[thread_count - 1]);

		std::ranges::for_each(threads, [](std::thread& t) { t.join(); });
		std::ranges::for_each(results, [&result](const mpz_class* r) { result *= *r; });
	}

	const clock_t end{ clock() };
//	fmt::println("Time: {}s", static_cast<double>(end - start) / CLOCKS_PER_SEC);
//	fmt::println("{:^21}", "5000000");
//	fmt::println("{:<12}{}", "i5-13400", "17.696s");
//	fmt::println("{:<12}{}", "i7-1165g7", "73.829s");
//	fmt::println("{:<12}{}", "i5-10210u", "267.419s");
//	fmt::println("{:<12}{}", "i7-8750h", "60.206s");
//	fmt::println("{:<12}{}", "i5-1145G7", "83.32s");
//	fmt::println("{:^21}", "6000000");
//	fmt::println("{:<12}{}", "i5-13400", "27.107s");
//	fmt::println("{:<12}{}", "i7-1165g7", "116.028s");
//	fmt::println("{:<12}{}", "i5-10210u", "417.6s");
//	fmt::println("{:<12}{}", "i7-8750h", "106.208s");
//	fmt::println("{:<12}{}", "i5-1145G7", "208.006s");
	system("pause");
	return 0;
}
