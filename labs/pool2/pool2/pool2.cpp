// pool2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <fstream>
#include <iostream>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

typedef std::vector<std::vector<int>> Pool;

struct Point
{
	int x;
	int y;
};

enum Direction
{
	DR,
	DL,
	UR,
	UL,
};

bool ValidateFile(const std::ifstream& inputFile);

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}

	int M;
	int N;
	inputFile >> M >> N;

	Pool pool;
	for (size_t i = 0; i < M; ++i)
	{
		std::vector<int> row;
		for (size_t j = 0; j < N; ++j)
		{
			int cell;
			inputFile >> cell;
			row.push_back(cell);
		}
		pool.push_back(row);
	}

	std::vector<Point> startPoints;
	startPoints.push_back({ 0, 0 });
	startPoints.push_back({ 0, N - 1 });
	startPoints.push_back({ M - 1, 0 });
	startPoints.push_back({ M - 1, N - 1 });

	for (auto& startPoint: startPoints)
	{
		
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
