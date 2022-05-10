// pool2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

struct Point
{
	int x;
	int y;

	friend bool operator==(const Point p1, const Point p2)
	{
		return p1.x == p2.x && p1.y == p2.y;
	}
};

struct Cell
{
	int value;
	int mark = 0;
};

enum class Direction
{
	DR,
	DL,
	UR,
	UL,
};

struct StartPoint
{
	Point position;
	Direction startDirection;
	Direction endDirection;
};

typedef std::vector<std::vector<Cell>> Pool;
bool ValidateFile(const std::ifstream& inputFile);
Point MakeSimpleMove(Point& point, Direction& startDirection);
Point MakeCommonMove(int M, int N, Point& point, Direction& startDirection);

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
		std::vector<Cell> row;
		for (size_t j = 0; j < N; ++j)
		{
			int cell;
			inputFile >> cell;
			row.push_back({ cell });
		}
		pool.push_back(row);
	}
	std::vector<Point> startPositions;
	startPositions.push_back({ 0, 0 });
	startPositions.push_back({ 0, N - 1 });
	startPositions.push_back({ M - 1, 0 });
	startPositions.push_back({ M - 1, N - 1 });
	
	std::vector<StartPoint> startPoints;
	startPoints.push_back({ startPositions[0], Direction::DR, Direction::UL });
	startPoints.push_back({ startPositions[1], Direction::DL, Direction::UR });
	startPoints.push_back({ startPositions[2], Direction::UR, Direction::DL });
	startPoints.push_back({ startPositions[3], Direction::UL, Direction::DR });

	int maxPts = 0;
	int maxCounter = 0;
	Point maxPosition;
	for (auto& startPoint: startPoints)
	{
		int counter = &startPoint - &startPoints[0];
		int currPts = 0;
		Point currPosition;
		Direction currDirection;
		currPts += pool[startPoint.position.x][startPoint.position.y].value;
		currDirection = startPoint.startDirection;
		currPosition = MakeSimpleMove(startPoint.position, currDirection);
		
		if (maxPts < currPts)
		{
			maxPts = currPts;
			maxPosition = currPosition;
			maxCounter = counter;
		}
		pool[startPoint.position.x][startPoint.position.y].mark = counter + 1;

		auto it = std::find(startPositions.begin(), startPositions.end(), currPosition);
		while (!(it != startPositions.end() && currDirection == startPoints[it - startPositions.begin()].endDirection) &&
			pool[currPosition.x][currPosition.y].mark < counter + 1)
		{
			currPts += pool[currPosition.x][currPosition.y].value;
			pool[currPosition.x][currPosition.y].mark = counter + 1;
			if (maxPts < currPts)
			{
				maxPts = currPts;
				maxPosition = currPosition;
				maxCounter = counter;
			}
			currPosition = MakeCommonMove(M, N, currPosition, currDirection);
			it = std::find(startPositions.begin(), startPositions.end(), currPosition);
		}
		currPts += pool[currPosition.x][currPosition.y].value;
		pool[currPosition.x][currPosition.y].mark = counter + 1;
		if (maxPts < currPts)
		{
			maxPts = currPts;
			maxPosition = currPosition;
			maxCounter = counter;
		}
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

Point MakeSimpleMove(Point& point, Direction& startDirection)
{
	Point result = point;

	switch (startDirection)
	{
	case Direction::DL:
		result.x = point.x + 1;
		result.y = point.y - 1;
		startDirection = Direction::DL;
		break;
	case Direction::DR:
		result.x = point.x + 1;
		result.y = point.y + 1;
		startDirection = Direction::DR;
		break;
	case Direction::UL:
		result.x = point.x - 1;
		result.y = point.y - 1;
		startDirection = Direction::UL;
		break;
	case Direction::UR:
		result.x = point.x - 1;
		result.y = point.y + 1;
		startDirection = Direction::UR;
		break;
	}

	return result;
}

Point MakeCommonMove(int M, int N, Point& point, Direction& startDirection)
{
	Point result = point;

	if (point.y == 0)
	{
		switch (startDirection)
		{
		case Direction::DL:
			result.x++;
			startDirection = Direction::DR;
			return result;
		case Direction::UL:
			result.x--;
			startDirection = Direction::UR;
			return result;
		}
	}
	if (point.x == 0)
	{
		switch (startDirection)
		{
		case Direction::UL:
			result.y--;
			//result.x = point.x - 1;
			startDirection = Direction::DL;
			return result;
		case Direction::UR:
			result.y++;
			//result.x = point.x + 1;
			startDirection = Direction::DR;
			return result;
		}
	}
	if (point.x == M - 1)
	{
		switch (startDirection)
		{
		case Direction::DL:
			result.y--;
			startDirection = Direction::UL;
			return result;
		case Direction::DR:
			result.y++;
			startDirection = Direction::UR;
			return result;
		}
	}
	if (point.y == N - 1)
	{
		switch (startDirection)
		{
		case Direction::DR:
			result.x++;
			startDirection = Direction::DL;
			return result;
		case Direction::UR:
			result.x--;
			startDirection = Direction::UL;
			return result;
		}
	}
	
	return MakeSimpleMove(point, startDirection);
}
