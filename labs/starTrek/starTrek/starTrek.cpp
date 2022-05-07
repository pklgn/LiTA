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

constexpr int START_PLANET_POS = 0;
constexpr int NO_NEXT_POSITION = INT_MIN;
constexpr int NO_PREVIOUS_POSITION = INT_MAX;

struct Planet
{
	int fuel;
	int prevPosition = INT_MAX;
	int nextPosition = INT_MIN;
	bool reachable = true;
};

typedef std::vector<Planet> Planets;
typedef std::unordered_map<int, int> FuelPosition;

bool ValidateFile(const std::ifstream& inputFile);
void CalculateRoute(int N, Planets& planets);
void PrintRoute(std::ostream& outputStream, Planets& planets);
void ReadPlanets(std::istream& inputStream, int N, Planets& planets, FuelPosition prevFuelPosition);

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

	FuelPosition prevFuelPosition;
	prevFuelPosition.reserve(N);

	ReadPlanets(inputFile, N, planets, prevFuelPosition);

	CalculateRoute(N, planets);

	std::ofstream outputFile(OUTPUT_FILE_NAME);

	PrintRoute(outputFile, planets);

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

int FindStopPlanet(int currPlanet, int nextPlanet, int N, Planets& planets, int prevCurrPlanet)
{
	int result = NO_NEXT_POSITION;
	int farawayPlanet = NO_NEXT_POSITION;

	if (nextPlanet == (N - 1))
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

void PrintRoute(std::ostream& outputStream, Planets& planets)
{
	if (planets[planets.size() - 1].prevPosition != NO_PREVIOUS_POSITION)
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
		outputStream << length << std::endl;

		for (size_t i = 0; i < length; ++i)
		{
			outputStream << results.top() + 1;
			results.pop();
			if (pos != lastPos)
			{
				outputStream << " ";
			}
			else
			{
				outputStream << std::endl;
			}
		}
	}
	else
	{
		outputStream << 0 << std::endl;
	}

	return;
}

void ReadPlanets(std::istream& inputStream, int N, Planets& planets, FuelPosition prevFuelPosition)
{
	for (int i = 0; i < N; ++i)
	{
		int planetFuel;
		inputStream >> planetFuel;
		planets.push_back({ planetFuel });

		if (prevFuelPosition.find(planetFuel) != prevFuelPosition.end())
		{
			planets[prevFuelPosition[planetFuel]].nextPosition = i;
		}
		prevFuelPosition[planetFuel] = i;
	}

	return;
}

void CalculateRoute(int N, Planets& planets)
{
	int currPlanetPos = START_PLANET_POS;
	int nextPlanetPos;
	int prevCurrPlanetPos = START_PLANET_POS;

	while (planets[N - 1].prevPosition == INT_MAX && planets[START_PLANET_POS].reachable)
	{
		nextPlanetPos = planets[currPlanetPos].nextPosition;
		if (nextPlanetPos == NO_NEXT_POSITION)
		{
			planets[currPlanetPos].reachable = false;
			currPlanetPos = planets[currPlanetPos].prevPosition;
			prevCurrPlanetPos = currPlanetPos;
			continue;
		}
		int stopPlanet = FindStopPlanet(currPlanetPos, nextPlanetPos, N, planets, prevCurrPlanetPos);

		if (stopPlanet == NO_NEXT_POSITION)
		{
			break;
		}
		else
		{
			currPlanetPos = stopPlanet;
			prevCurrPlanetPos = nextPlanetPos;
		}
	}

	return;
}