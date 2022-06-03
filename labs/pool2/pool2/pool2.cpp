/*
* 4.3. Бильярд 2 (6)
* Бильярдный стол расчерчен на квадратные клетки и имеет размеры M × N клеток  (2 ≤ M, N ≤ 500).
* В углах стола находятся четыре лузы для шаров. В каждой клетке (i, j), i = 1, 2,..., M; j = 1, 2,..., N
* задано целое число Cij  ( -1000 ≤  Cij ≤ 1000). Шар ставится в центр одной из клеток и после удара может
* катиться в одном из четырех направлений вдоль диагоналей клетки. Если он достигает края стола, то отражается
* и продолжает движение, а если попадает в угол стола, то сваливается в лузу.
* Количество отражений не ограничено. 
* Требуется выбрать начальную клетку для шара и направление удара так, чтобы 
*	шар попал в одну из луз;
*	не пересек какую-либо клетку дважды по одной или разным диагоналям;
*	сумма чисел в клетках, которые шар пересек по диагонали, включая начальную клетку, была максимальной.
* 
* Если имеется несколько решений, достаточно дать любое из них.
* 
* 
* Стандарт C++ 14
* Среда разработки: Visual Studio 2019, MSVC
* ОС: Windows 10
*
* Выполнил: Ермаков Павел, ПС-21
*/

#include <vector>
#include <sstream>
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
	NO_DIRECTION,
};

struct StartPoint
{
	Point position;
	Direction startDirection;
	Direction endDirection;
};

struct State
{
	int pts = 0;
	int pocket = 0;
	Point position;
	Direction direction;
};

typedef std::vector<std::vector<Cell>> Pool;
typedef std::vector<Point> StartPositions;
typedef std::vector<StartPoint> StartPoints;

bool ValidateFile(const std::ifstream& inputFile);
Point MakeSimpleMove(Point& point, Direction& startDirection);
Point MakeCommonMove(int M, int N, Point& point, Direction& startDirection);
void UpdateMax(State& maxState, State& currState);
void ProcessMove(State& currState, State& maxState, Pool& pool);
void PrintResult(std::ostream& outputStream, State& maxState);
State FindStartState(int M, int N, Pool& pool, StartPoints& startPoints, StartPositions& startPositions);

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
	for (int i = 0; i < M; ++i)
	{
		std::vector<Cell> row;
		for (int j = 0; j < N; ++j)
		{
			int cell;
			inputFile >> cell;
			row.push_back({ cell });
		}
		pool.push_back(row);
	}
	StartPositions startPositions({ { 0, 0 },
		{ 0, N - 1 },
		{ M - 1, 0 },
		{ M - 1, N - 1 } });
	
	StartPoints startPoints({ { startPositions[0], Direction::DR, Direction::UL },
		{ startPositions[1], Direction::DL, Direction::UR },
		{ startPositions[2], Direction::UR, Direction::DL },
		{ startPositions[3], Direction::UL, Direction::DR } });

	State result = FindStartState(M, N, pool, startPoints, startPositions);

	std::ofstream outputFile(OUTPUT_FILE_NAME);
	PrintResult(outputFile, result);

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
			startDirection = Direction::DL;
			return result;
		case Direction::UR:
			result.y++;
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

Direction GetOppositeDirection(Direction direction)
{
	switch (direction)
	{
	case Direction::DL:
		return Direction::UR;
	case Direction::DR:
		return Direction::UL;
	case Direction::UL:
		return Direction::DR;
	case Direction::UR:
		return Direction::DL;
	default:
		return Direction::NO_DIRECTION;
	}
}

std::string DirectionToString(Direction direction)
{
	std::ostringstream stringStream;

	switch (direction)
	{
	case Direction::DL:
		stringStream << "DL";
		break;
	case Direction::DR:
		stringStream << "DR";
		break;
	case Direction::UL:
		stringStream << "UL";
		break;
	case Direction::UR:
		stringStream << "UR";
		break;
	}

	return stringStream.str();
}

void PrintResult(std::ostream& outputStream, State& maxState)
{
	outputStream << maxState.pts << std::endl
				 << maxState.position.x + 1 << " " << maxState.position.y + 1 << std::endl
				 << DirectionToString(maxState.direction) << std::endl;

	return;
}

void UpdateMax(State& maxState, State& currState)
{
	if (maxState.pts < currState.pts)
	{
		maxState.pts = currState.pts;
		maxState.position = currState.position;
		maxState.pocket = currState.pocket;
		maxState.direction = GetOppositeDirection(currState.direction);
	}

	return;
}

void ProcessMove(State& currState, State& maxState, Pool& pool)
{
	currState.pts += pool[currState.position.x][currState.position.y].value;
	pool[currState.position.x][currState.position.y].mark = currState.pocket + 1;
	UpdateMax(maxState, currState);

	return;
}

State FindStartState(int M, int N, Pool& pool, StartPoints& startPoints, StartPositions& startPositions)
{
	State maxState;
	for (auto& startPoint : startPoints)
	{
		State currState;
		currState.pocket = &startPoint - &startPoints[0];
		currState.pts += pool[startPoint.position.x][startPoint.position.y].value;
		currState.direction = startPoint.startDirection;
		currState.position = MakeSimpleMove(startPoint.position, currState.direction);

		UpdateMax(maxState, currState);
		pool[startPoint.position.x][startPoint.position.y].mark = currState.pocket + 1;

		auto it = std::find(startPositions.begin(), startPositions.end(), currState.position);
		while (!(it != startPositions.end() && currState.direction == startPoints[it - startPositions.begin()].endDirection) && pool[currState.position.x][currState.position.y].mark < currState.pocket + 1)
		{
			ProcessMove(currState, maxState, pool);

			currState.position = MakeCommonMove(M, N, currState.position, currState.direction);
			it = std::find(startPositions.begin(), startPositions.end(), currState.position);
		}
		ProcessMove(currState, maxState, pool);
	}

	return maxState;
}