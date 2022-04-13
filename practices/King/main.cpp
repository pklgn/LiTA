#include <iostream>
#include <fstream>
#include <vector>

struct Position
{
	int x;
	int y;
};

int main(int argc, char* argv[])
{
	std::string inputFileName = argv[1];
	std::ifstream inputFile(inputFileName);

	int N;
	inputFile >> N;
	std::vector<Position> startPositions;
	for (int i = 0; i < N; ++i)
	{
		Position position;
		inputFile >> position.x;
		inputFile >> position.y;
		startPositions.push_back(position);
	}
	int winner;

	for (auto startPosition: startPositions)
	{
		if (startPosition.x % 2 == 0)
		{
			winner = 1;
		}
		else
		{
			if (startPosition.y % 2 == 0)
			{
				winner = 1;
			}
			else
			{
				winner = 2;
			}
		}
		std::cout << winner << std::endl;
	}

	return 0;
}