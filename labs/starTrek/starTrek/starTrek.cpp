// starTrek.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stack>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <unordered_map>
#include <vector>

const std::string INPUT_FILE_NAME = "input.txt";
const std::string OUTPUT_FILE_NAME = "output.txt";

bool ValidateFile(const std::ifstream& inputFile);

struct Planet
{
	int fuel;
	int prevPosition = INT_MAX;
	int nextPosition = INT_MIN;
	bool reachable = true;
};

typedef std::vector<Planet> Planets;

int FindStopPlanet(int currPlanet, int nextPlanet, Planets& planets, int prevCurrPlanet);

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
	std::vector<int> test;

	std::unordered_map<int, int> fuelPositions;
	fuelPositions.reserve(N);

	for (size_t i = 0; i < N; ++i)
	{
		int planetFuel;
		inputFile >> planetFuel;
		planets.push_back({ planetFuel });
		
		if (fuelPositions.find(planetFuel) != fuelPositions.end())
		{
			planets[fuelPositions[planetFuel]].nextPosition = i;
		}
		fuelPositions[planetFuel] = i;
	}

	int currPlanetPos = 0;
	int nextPlanetPos;
	int prevCurrPlanetPos = 0;

	while (planets[planets.size() - 1].prevPosition == INT_MAX && planets[0].reachable)
	{
		nextPlanetPos = planets[currPlanetPos].nextPosition;
		if (nextPlanetPos == INT_MIN)
		{
			planets[currPlanetPos].reachable = false;
			currPlanetPos = planets[currPlanetPos].prevPosition;
			prevCurrPlanetPos = currPlanetPos;
			continue;
		}
		int stopPlanet = FindStopPlanet(currPlanetPos, nextPlanetPos, planets, prevCurrPlanetPos);

		if (stopPlanet == INT_MIN)
		{
			break;
		}
		else
		{
			currPlanetPos = stopPlanet;
			prevCurrPlanetPos = nextPlanetPos;
		}

	}

	std::ofstream outputFile(OUTPUT_FILE_NAME);

	if (planets[planets.size() - 1].prevPosition != INT_MAX)
	{
		std::stack<int> results;
		int pos = planets[planets.size() - 1].prevPosition;
		int lastPos = pos;
		results.push(pos);
		while (pos != 0)
		{
			pos = planets[pos].prevPosition;
			results.push(pos);
		}
		size_t length = results.size();
		outputFile << length << std::endl;
		for (size_t i = 0; i < length; ++i)
		{
			outputFile << results.top() + 1;
			results.pop();
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

int FindStopPlanet(int currPlanet, int nextPlanet, Planets& planets, int prevCurrPlanet)
{
	int result = INT_MIN;
	int farawayPlanet = INT_MIN;

	if (nextPlanet == (planets.size() - 1))
	{
		planets[nextPlanet].prevPosition = currPlanet;

		return result;
	}

	for (int pos = prevCurrPlanet; pos <= nextPlanet; ++pos)
	{
		if (farawayPlanet < planets[pos].nextPosition && planets[pos].reachable && pos != currPlanet)
		{
			planets[pos].prevPosition = currPlanet;
			farawayPlanet = planets[pos].nextPosition;
			result = pos;
		}
	}

	return result;
}