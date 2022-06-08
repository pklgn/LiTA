#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>

struct Point
{
	Point(size_t x, size_t y)
		: m_x(x)
		, m_y(y)
	{
	}

	size_t m_x;
	size_t m_y;
};

size_t Mark(size_t i, size_t j, std::vector<std::vector<char>>& garden);

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Wrong argument number\n";

		return 1;
	}

	std::ifstream inputFile(argv[1]);

	size_t M;
	size_t N;

	std::vector<std::vector<char>> garden;

	inputFile >> M >> N;

	std::vector<char> emptyGardenLine;
	for (size_t i = 0; i < N + 2; ++i)
	{
		emptyGardenLine.push_back('.');
	}

	garden.push_back(emptyGardenLine);
	for (size_t i = 0; i < M; ++i)
	{
		std::vector<char> gardenLine;
		gardenLine.push_back('.');
		for (size_t j = 0; j < N; ++j)
		{
			char ceilCh;
			inputFile >> ceilCh;
			gardenLine.push_back(ceilCh);
		}
		gardenLine.push_back('.');
		garden.push_back(gardenLine);
	}
	garden.push_back(emptyGardenLine);


	size_t resultLength = 0;
	for (size_t i = 0; i < M; ++i)
	{
		size_t currResultLength = 0;
		for (size_t j = 0; j < N; ++j)
		{
			if (garden[i][j] == '#')
			{
				currResultLength = Mark(i, j, garden);

				resultLength = std::max(currResultLength, resultLength);
			}

		}
	}

	std::cout << resultLength << std::endl;

	return 0;
}

size_t Mark(size_t i, size_t j, std::vector<std::vector<char>>& garden)
{
	size_t resultLength = 0;
	if (garden[i][j] == '#')
	{
		resultLength += 1;
		garden[i][j] = '.';
		std::array<size_t, 5> adjLength;
		std::vector<std::vector<char>> tempGarden = garden;
		tempGarden[i][j] = '.';
		adjLength[0] = Mark(i - 1, j, tempGarden);
		adjLength[1] = Mark(i + 1, j, tempGarden);
		adjLength[2] = Mark(i, j - 1, tempGarden);
		adjLength[3] = Mark(i, j + 1, tempGarden);
		
		adjLength[4] = resultLength;
		auto maxIndex = std::distance(adjLength.begin(), std::max_element(adjLength.begin(), adjLength.end()));
		auto maxLength = *std::max_element(adjLength.begin(), adjLength.end());
		size_t maxLength2 = 0;
		for (size_t i = 0; i < 5; ++i)
		{
			if (adjLength[i] > maxLength2 && i != maxIndex)
			{
				maxLength2 = adjLength[i];
			}
		}

		resultLength = maxLength + maxLength2;
	}

	return resultLength;
}