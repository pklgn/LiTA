#include <fstream>
#include <iostream>
#include <deque>

int main(int argc, char** argv)
{
	std::ifstream inputFile(argv[1]);
	if (!inputFile.is_open())
	{
		std::cout << "Can't open file\n";

		return 1;
	}

	unsigned long long int N;
	inputFile >> N;

	if (N % 2 == 0)
	{
		std::cout << "No\n";

		return 0;
	}

	std::deque<short> results; 
	while (N != 1)
	{
		unsigned long long int K1 = (N - 1) / 2;
		unsigned long long int K2 = (N + 1) / 2;
		if (K1 % 2 == 0 && K2 % 2 == 0)
		{
			std::cout << "No\n";

			return 0;
		}
		if (K1 % 2 != 0)
		{
			N = K1;
			results.push_front(2);
		}
		else if (K2 % 2 != 0)
		{
			N = K2;
			results.push_front(1);
		}
	}
	std::cout << results.size() << std::endl;
	for (auto& result: results)
	{
		std::cout << result << " ";
	}
	std::cout << std::endl;

	return 0;
}