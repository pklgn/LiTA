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
	int RectangleId;
	PointType pointType;
	SideType sideType;
};

struct Dimensions
{
	int width;
	int height;
};
// pos PointType
typedef std::map<int, std::vector<VertexInfo>> AxisPosition;
typedef std::map<int, AxisPosition> EventPointsMap;

Dimensions GetDimensions(Point& A, Point& C);
bool ValidateFile(const std::ifstream& inputFile);
void InsertVertexPoint(int axis, EventPointsMap& axisPoints, int axisPosition,
	const VertexInfo& vertexInfo);

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

	EventPointsMap yPoints;
	EventPointsMap xPoints;

	for (int rectangleId = 0; rectangleId < N; ++rectangleId)
	{
		Point A;
		Point C;
		inputFile >> A >> C;
		Dimensions dimensions = GetDimensions(A, C);
		Point B = { A.x + dimensions.width, A.y };
		Point D = { A.x, A.y + dimensions.height };
		InsertVertexPoint(A.y, yPoints, A.x, { rectangleId, PointType::Begin, SideType::Up });
		InsertVertexPoint(A.y, yPoints, B.x, { rectangleId, PointType::End, SideType::Up });
		InsertVertexPoint(C.y, yPoints, C.x, { rectangleId, PointType::End, SideType::Down });
		InsertVertexPoint(C.y, yPoints, D.x, { rectangleId, PointType::Begin, SideType::Down });
		InsertVertexPoint(A.x, xPoints, A.y, { rectangleId, PointType::End, SideType::Left });
		InsertVertexPoint(B.x, xPoints, B.y, { rectangleId, PointType::End, SideType::Right });
		InsertVertexPoint(C.x, xPoints, C.y, { rectangleId, PointType::Begin, SideType::Right });
		InsertVertexPoint(D.x, xPoints, D.y, { rectangleId, PointType::Begin, SideType::Left });
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

void InsertVertexPoint(int axis, EventPointsMap& axisPoints, int axisPosition,
	const VertexInfo& vertexInfo)
{
	axisPoints[axis][axisPosition].push_back(vertexInfo);
}

void FindIntersectionPoints(EventPointsMap& mainPoints, EventPointsMap& axisPoints)
{
	for (auto& mainPoint : mainPoints)
	{
		int state = 0;
		SideType currSide;
		PointType currPoint;

		for (auto& axisPoint : axisPoints)
		{
			for (auto& scanLine : axisPoint.second)
			{
				for (auto& vertexPoint : scanLine.second)
				{
					if (vertexPoint.pointType == PointType::Begin)
					{
						state += 1;
						currSide = vertexPoint.sideType;
					}
					else if (vertexPoint.pointType == PointType::End)
					{
						state -= 1;
					}
				}
			}
			if (mainPoints.count(axisPoint.first))
			{
				//if (state)
			}
		}
	}
	}