// starTrek.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

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

	for (size_t i = 0; i < N; ++i)
	{
		size_t planetFuel;
		inputFile >> planetFuel;
		planets.push_back({ planetFuel });
	}

	std::deque<Planet*> planetTrip;
	planetTrip.push_back(&planets[0]);
	planetTrip.front()->depth = 0;
	int i = 0;
	while (!planetTrip.empty())
	{
		if (i < planets.size())
		{
			++i;
		}
		

		while (i < planets.size() && planets[i].fuel != planetTrip.front()->fuel)
		{
			planetTrip.push_back(&planets[i]);
			++i;
		}
		if (i < planets.size())
		{
			planetTrip.push_back(&planets[i]);
			for (size_t j = 1; j < planetTrip.size() && planetTrip.front()->fuel == planetTrip.back()->fuel; ++j)
			{
				if (planetTrip.front()->depth + 1 < planetTrip[j]->depth && planetTrip.front() != planetTrip[j])
				{
					planetTrip[j]->prevPosition = planetTrip.front() - &planets[0];
					planetTrip[j]->depth = planetTrip.front()->depth + 1;
				}
			}
			if (!planetTrip.empty())
			{
				planetTrip.pop_front();
			}
		}
		else if (planets[i - 1].fuel != planetTrip.front()->fuel)
		{
			planetTrip.pop_front();
		}
		else
		{
			for (size_t j = 1; j < planetTrip.size() && planetTrip.front()->fuel == planetTrip.back()->fuel; ++j)
			{
				if (planetTrip.front()->depth + 1 < planetTrip[j]->depth && planetTrip.front() != planetTrip[j])
				{
					planetTrip[j]->prevPosition = planetTrip.front() - &planets[0];
					planetTrip[j]->depth = planetTrip.front()->depth + 1;
				}
			}
			if (!planetTrip.empty())
			{
				planetTrip.pop_front();
			}
		}
	}

	

	if (planets[planets.size() - 1].depth != INT_MAX)
	{
		std::deque<size_t> result;
		size_t pos = planets[planets.size() - 1].prevPosition;
		while (pos != -1)
		{
			result.push_front(pos);
			pos = planets[pos].prevPosition;
		}
		std::cout << result.size() << std::endl;
		for (auto& pos: result)
		{
			std::cout << pos;
			if (pos != -1)
			{
				std::cout << " ";
			}
			else 
			{
				std::cout << std::endl;
			}
		}
	}
	else 
	{
		std::cout << 0 << std::endl;
	}
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