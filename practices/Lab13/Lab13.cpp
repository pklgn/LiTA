// Lab13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char* argv[])
{
	std::string inputFileName = argv[1];
	std::ifstream inputFile(inputFileName);

	int V;
	inputFile >> V;
	std::vector<std::string> result;
	for (int i = 0; i < V; ++i)
	{
		int N;
		inputFile >> N;
		std::vector<int> numbers;
		for (int j = 0; j < N; ++j)
		{
			int number;
			inputFile >> number;
			numbers.push_back(number);
		}
		std::sort(numbers.begin(), numbers.end());
		int d = INT_MIN;
		for (int j = 0; j < N - 1; ++j)
		{
			int currD = numbers[j + 1] - numbers[j];
			if (d == INT_MIN)
			{
				d = currD;
			}
			if (currD != d)
			{
				result.push_back("No");
				break;
			}
		}
		if (result.size() < i + 1)
		{
			result.push_back("Yes");
		}
	}

	for (auto& answer: result)
	{
		std::cout << answer << std::endl;
	}

	return 0;
}
