#include <gmpxx.h>
#include <iostream>
#include <ctime>
#include <thread>
#include <vector>
#include <fmt/format.h>

void Factorial(const mpz_class& start, const mpz_class& end, mpz_class* out)
{
	mpz_class result = start;
	for (mpz_class i = start + 1; i <= end; i += 1)
	{
		result *= i;
	}
	*out = result;
}

int main(void)
{
	unsigned long number = 5000;
	int thread_type{ 1 };
	fmt::print("Number: ");
	std::cin >> number;

	const clock_t start{ clock() };

	mpz_class result = 1;
	if (thread_type == 0)
	{
		Factorial(1, number, &result);
	}
	else if (thread_type == 1)
	{
		// multi thread
		const unsigned thread_count{ std::thread::hardware_concurrency() > number ? number : std::thread::hardware_concurrency() };
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

	fmt::println("result: {}", result);

	const clock_t end{ clock() };
	fmt::println("Time: {}s", static_cast<double>(end - start) / CLOCKS_PER_SEC);
	system("pause");
}
