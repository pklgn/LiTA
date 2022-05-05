// starTrek.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <deque>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <map>
#include <vector>

const std::string INPUT_FILE_NAME = "input.txt";
const std::string OUTPUT_FILE_NAME = "output.txt";

bool ValidateFile(const std::ifstream& inputFile);

struct Planet
{
	size_t fuel;
	int prevPosition = -1;
	int depth = INT_MAX;
};

typedef std::vector<Planet> Planets;

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}

	size_t N;
	inputFile >> N;

	Planets planets;
	planets.reserve(N);

	for (size_t i = 0; i < N; ++i)
	{
		size_t planetFuel;
		inputFile >> planetFuel;
		planets.push_back({ planetFuel });
	}

	std::map<int, int> fuelState;
	size_t q = 0;

	planets[0].depth = 0;
	fuelState[planets[0].fuel] += 1;
	size_t i = 1;
	bool exitFlag = false;

	while (i - q != 0 && !exitFlag)
	{
		while (i < planets.size() && fuelState[planets[q].fuel] <= 1)
		{
			
			if (planets[i].fuel == planets[q].fuel && (i - q) != 1)
			{
				fuelState[planets[i].fuel] += 1;
				++i;
				break;
			}
			fuelState[planets[i].fuel] += 1;
			++i;
		}

		bool isExist = fuelState[planets[q].fuel] > 1;
		for (size_t j = q + 1; j <= i && isExist; ++j)
		{
			if (planets[q].depth + 1 < planets[j].depth)
			{
				planets[j].prevPosition = &planets[q] - &planets[0];
				planets[j].depth = planets[q].depth + 1;
			}
			if (planets[q].fuel == planets[j].fuel)
			{
				break;
			}
		}

		if (q < i)
		{
			fuelState[planets[q].fuel] -= 1;

			++q;
			if (q <= i)
			{
				if (planets[q].prevPosition == -1)
				{
					exitFlag = true;
				}
			}
		}

		if (i == planets.size() && fuelState[planets[i - 1].fuel] < 2)
		{
			break;
		}
	}

	std::ofstream outputFile(OUTPUT_FILE_NAME);

	if (planets[planets.size() - 1].prevPosition != -1)
	{
		std::deque<int> result;
		int pos = planets[planets.size() - 1].prevPosition;
		int lastPos = pos;

		while (pos != -1)
		{
			result.push_front(pos);
			pos = planets[pos].prevPosition;
		}
		outputFile << result.size() << std::endl;
		for (auto& pos : result)
		{
			outputFile << pos + 1;
			if (pos != lastPos)
			{
				outputFile << " ";
			}
			else
			{
				outputFile << std::endl;
			}
		}
	}
	else
	{
		outputFile << 0 << std::endl;
	}

	return 0;
}

bool ValidateFile(const std::ifstream& inputFile)
{
	if (!inputFile.is_open())
	{
		std::cout << "Failed to open input file for reading\n";

		return false;
	}

	return true;
}