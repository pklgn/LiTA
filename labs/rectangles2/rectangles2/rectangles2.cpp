// rectangles2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

typedef std::pair<int, int> RectangleSide;

enum class PointType
{
	Begin,
	End,
	IntersectionBegin,
	IntersectionEnd,
};

enum class SideType
{
	Left,
	Up,
	Right,
	Down,
};

struct Point
{
	int x;
	int y;
	PointType type;

	friend std::istream& operator>>(std::istream& is, Point& point)
	{
		is >> point.x >> point.y;
		return is;
	}
};

struct VertexInfo
{
	int rectangleId;
	PointType pointType;
};

struct Dimensions
{
	int width;
	int height;
};

struct Rectangle
{
	Point LUVertex;
	Dimensions dimensions;
};

typedef std::vector<Rectangle> Rectagles;
typedef std::map<int, std::vector<VertexInfo>> EventPointsMap;

Dimensions GetDimensions(Point& A, Point& C);
bool ValidateFile(const std::ifstream& inputFile);
void InsertVertexPoint(int axis, EventPointsMap& axisPoints, int axisPosition,
	const VertexInfo& vertexInfo);
void FindIntersectionPoints(EventPointsMap& mainPoints, EventPointsMap& axisPoints);

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}

	int N; //rectangles number
	inputFile >> N;

	Rectagles rectangles;
	EventPointsMap yPoints;
	EventPointsMap xPoints;

	for (int rectangleId = 0; rectangleId < N; ++rectangleId)
	{
		Point A;
		Point C;
		inputFile >> A >> C;
		Dimensions dimensions = GetDimensions(A, C);
		Point B = { A.x + dimensions.width, A.y };
		rectangles.push_back({ A, dimensions });
		
		yPoints[A.x].push_back({ rectangleId, PointType::Begin });
		yPoints[B.x].push_back({ rectangleId, PointType::End });
		xPoints[C.y].push_back({ rectangleId, PointType::Begin });
		xPoints[B.y].push_back({ rectangleId, PointType::End });
	}

	EventPointsMap currXPoints;
	for (auto& yPoint : yPoints)
	{
		for (auto& vertex: yPoint.second)
		{
			if (vertex.pointType == PointType::Begin)
			{
				const Rectangle rectangle = rectangles[vertex.rectangleId];
				currXPoints[rectangle.LUVertex.y].push_back({ vertex.rectangleId, PointType::End });
				currXPoints[rectangle.LUVertex.y - rectangle.dimensions.height].push_back({ vertex.rectangleId, PointType::Begin });
			}
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

Dimensions GetDimensions(Point& A, Point& C)
{
	Dimensions result;
	result.width = C.x - A.x;
	result.height = C.y - A.y;

	return result;
}