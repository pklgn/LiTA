// starTrek.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <limits.h>

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
	//std::ifstream inputFile(INPUT_FILE_NAME);
	//bool status = ValidateFile(inputFile);
	//if (!status)
	//{
	//	return 1;
	//}

	size_t N;
	std::cin >> N;

	Planets planets;

	for (size_t i = 0; i < N; ++i)
	{
		size_t planetFuel;
		std::cin >> planetFuel;
		planets.push_back({ planetFuel });
	}

	std::deque<Planet*> planetTrip;
	planetTrip.push_back(&planets[0]);
	planetTrip.front()->depth = 0;
	planetTrip.push_back(&planets[1]);
	size_t i = 2;
	bool exitFlag = false;
	while (!planetTrip.empty() && !exitFlag)
	{
		while (i < planets.size())
		{
			if (planetTrip.front()->fuel == planetTrip.back()->fuel && planetTrip.size() != 1)
			{
				break;
			}
			else if (planets[i].fuel == planetTrip.front()->fuel)
			{
				planetTrip.push_back(&planets[i]);
				++i;
				break;
			}
			planetTrip.push_back(&planets[i]);
			++i;
		}

		auto it = std::find_if(planetTrip.begin(), planetTrip.end(),
			[planetTrip](Planet* planet) { return planetTrip.front()->fuel == planet->fuel && planetTrip.front() != planet; });
		bool status = it != planetTrip.end();
		for (size_t j = 1; j < planetTrip.size() && status; ++j)
		{
			
			if (planetTrip.front()->depth + 1 < planetTrip[j]->depth)
			{
				planetTrip[j]->prevPosition = planetTrip.front() - &planets[0];
				planetTrip[j]->depth = planetTrip.front()->depth + 1;
			}
			if (planetTrip.front()->fuel == planetTrip[j]->fuel)
			{
				break;
			}
		}

		if (!planetTrip.empty())
		{
			planetTrip.pop_front();
			if (!planetTrip.empty())
			{
				if (planetTrip.front()->prevPosition == -1)
				{
					exitFlag = true;
				}
			}
		}
	}

	if (planetTrip.empty())
	{
		std::deque<int> result;
		int pos = planets[planets.size() - 1].prevPosition;
		int lastPos = pos;
		
		while (pos != -1)
		{
			result.push_front(pos);
			pos = planets[pos].prevPosition;
		}
		std::cout << result.size() << std::endl;
		for (auto& pos: result)
		{
			std::cout << pos + 1;
			if (pos != lastPos)
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