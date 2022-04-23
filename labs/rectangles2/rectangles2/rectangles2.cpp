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
	BeginEnd,
	IntersectionBegin,
	IntersectionEnd,
	IntersectionBeginEnd,
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
	int count = 0;
	PointType pointType;
	SideType sideType;
};

struct Dimensions
{
	int width;
	int height;
};
// pos PointType
typedef std::map<int, VertexInfo> VertexPoint;
typedef std::map<int, VertexPoint> EventPoints;

Dimensions GetDimensions(Point& A, Point& C);
bool ValidateFile(const std::ifstream& inputFile);
void InsertVertexPoint(int axis, EventPoints& axisPoints, int axisPosition, const VertexInfo& vertexInfo, EventPoints& normalAxisPoints);

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

	EventPoints yPoints;
	EventPoints xPoints;

	for (int i = 0; i < N; ++i)
	{
		Point A;
		Point C;
		inputFile >> A >> C;
		Dimensions dimensions = GetDimensions(A, C);
		Point B = { A.x + dimensions.width, A.y };
		Point D = { A.x, A.y + dimensions.height };

		InsertVertexPoint(A.y, yPoints, A.x, { 1, PointType::Begin, SideType::Up }, xPoints);
		InsertVertexPoint(A.y, yPoints, B.x, { 1, PointType::End, SideType::Up }, xPoints);
		InsertVertexPoint(C.y, yPoints, C.x, { 1, PointType::End, SideType::Down }, xPoints);
		InsertVertexPoint(C.y, yPoints, D.x, { 1, PointType::Begin, SideType::Down }, xPoints);
		InsertVertexPoint(A.x, xPoints, A.y, { 1, PointType::End, SideType::Left }, yPoints);
		InsertVertexPoint(B.x, xPoints, B.y, { 1, PointType::End, SideType::Right }, yPoints);
		InsertVertexPoint(C.x, xPoints, C.y, { 1, PointType::Begin, SideType::Right }, yPoints);
		InsertVertexPoint(D.x, xPoints, D.y, { 1, PointType::Begin, SideType::Left }, yPoints);
	}

	for (auto& xPoint : xPoints)
	{
		int state = 0;
		SideType currSide;
		PointType currPoint;

		for (auto& yPoint : yPoints)
		{
			if (xPoint.second.count(yPoint.first) && xPoint.second[yPoint.first].count > 0)
			{
				if (xPoint.second[yPoint.first].pointType == PointType::Begin)
				{
					state += xPoint.second[yPoint.first].count;
				}
				else if (xPoint.second[yPoint.first].pointType == PointType::End)
				{
					state -= xPoint.second[yPoint.first].count;
				}

				currSide = xPoint.second[yPoint.first].sideType;
				if (currSide == SideType::Left)
				{
					currPoint = PointType::IntersectionBegin;
				}
				else if (currSide == SideType::Right)
				{
					currPoint = PointType::IntersectionEnd;
				}
			}

			if (state > 0)
			{
				yPoint.second.insert({ xPoint.first, { 1, currPoint, currSide } });
			}
		}
	}

	for (auto& yPoint : yPoints)
	{
		int state = 0;
		SideType currSide;
		PointType currPoint;

		for (auto& xPoint : xPoints)
		{
			if (yPoint.second.count(xPoint.first) && yPoint.second[xPoint.first].count > 0)
			{
				if (yPoint.second[xPoint.first].pointType == PointType::Begin)
				{
					state += yPoint.second[xPoint.first].count;
				}
				else if (yPoint.second[xPoint.first].pointType == PointType::End)
				{
					state -= yPoint.second[xPoint.first].count;
				}

				currSide = yPoint.second[xPoint.first].sideType;
				if (currSide == SideType::Down)
				{
					currPoint = PointType::IntersectionBegin;
				}
				else if (currSide == SideType::Up)
				{
					currPoint = PointType::IntersectionEnd;
				}
			}

			if (state > 0)
			{
				xPoint.second.insert({ yPoint.first, { 1, currPoint, currSide } });
			}
		}
	}
	int xResult = 0;

	std::pair<int, VertexInfo> lastSafePoint;
	for (auto& xPoint : xPoints)
	{
		int sideState = 0;
		int intersectionState = 0;
		for (auto& point : xPoint.second)
		{
			if (point.second.pointType == PointType::IntersectionBegin)
			{

				if (sideState > 0 && !intersectionState)
				{
					xResult += point.first - lastSafePoint.first;
				}
				intersectionState += 1;
			}
			else if (point.second.pointType == PointType::IntersectionEnd)
			{
				intersectionState -= 1;
				if (sideState > 0)
				{
					lastSafePoint = point;
				}
			}
			if (!intersectionState && point.second.count != -1)
			{
				if (point.second.pointType == PointType::Begin)
				{
					if (sideState == 0)
					{
						lastSafePoint = point;
					}
					sideState += point.second.count;
				}
				else if (point.second.pointType == PointType::End)
				{
					if (sideState > 1)
					{
						lastSafePoint = point;
					}
					sideState -= point.second.count;
					xResult += point.first - lastSafePoint.first;
				}
			}
			else if (intersectionState)
			{
				if (point.second.pointType == PointType::Begin)
				{
					sideState += point.second.count;
				}
				else if (point.second.pointType == PointType::End)
				{
					sideState -= point.second.count;
				}
			}
		}
	}

	int yResult = 0;

	for (auto& yPoint : yPoints)
	{
		int sideState = 0;
		int intersectionState = 0;
		std::pair<int, VertexInfo> lastSafePoint;
		for (auto& point : yPoint.second)
		{
			if (point.second.pointType == PointType::IntersectionBegin)
			{

				if (sideState > 0 && !intersectionState)
				{
					yResult += point.first - lastSafePoint.first;
				}
				intersectionState += point.second.count;
			}
			else if (point.second.pointType == PointType::IntersectionEnd)
			{
				intersectionState -= point.second.count;
				if (sideState > 0 && !intersectionState)
				{
					lastSafePoint = point;
				}
			}
			if (!intersectionState && point.second.count != -1)
			{
				if (point.second.pointType == PointType::Begin)
				{
					if (sideState == 0)
					{
						lastSafePoint = point;
					}
					sideState += point.second.count;
				}
				else if (point.second.pointType == PointType::End)
				{
					if (sideState > 1)
					{
						lastSafePoint = point;
					}
					sideState -= point.second.count;
					yResult += point.first - lastSafePoint.first;
				}
			}
			else if (intersectionState)
			{
				if (point.second.pointType == PointType::Begin)
				{
					sideState += point.second.count;
				}
				else if (point.second.pointType == PointType::End)
				{
					sideState -= point.second.count;
				}
			}
		}
	}

	int result = xResult + yResult;

	std::cout << result << std::endl;

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

void InsertVertexPoint(int axis, EventPoints& axisPoints, int axisPosition, const VertexInfo& vertexInfo, EventPoints& normalAxisPoints)
{
	if (axisPoints[axis].count(axisPosition))
	{
		if (axisPoints[axis][axisPosition].pointType != vertexInfo.pointType || axisPoints[axis][axisPosition].sideType != vertexInfo.sideType)
		{
			axisPoints[axis][axisPosition].count = -1;
		}
		else if (axisPoints[axis][axisPosition].count >= 0)
		{
			VertexInfo updatedVertexInfo = vertexInfo;
			updatedVertexInfo.count += axisPoints[axis][axisPosition].count;
			axisPoints[axis].insert({ axisPosition, updatedVertexInfo });
		}
	}
	else
	{
		axisPoints[axis].insert({ axisPosition, vertexInfo });
	}
}

void InsertIntersectionPoints(EventPoints& basicPoints, EventPoints& scanPoints)
{
	for (auto& basicPoint : basicPoints)
	{
		int state = 0;
		SideType currSide;
		PointType currPoint;

		for (auto& scanPoint : scanPoints)
		{
			if (basicPoint.second.count(scanPoint.first) && basicPoint.second[scanPoint.first].count > 0)
			{
				if (basicPoint.second[scanPoint.first].pointType == PointType::Begin)
				{
					state += basicPoint.second[scanPoint.first].count;
				}
				else if (basicPoint.second[scanPoint.first].pointType == PointType::End)
				{
					state -= basicPoint.second[scanPoint.first].count;
				}

				currSide = basicPoint.second[scanPoint.first].sideType;
				if (currSide == SideType::Left)
				{
					currPoint = PointType::IntersectionBegin;
				}
				else if (currSide == SideType::Right)
				{
					currPoint = PointType::IntersectionEnd;
				}
			}

			if (state > 0)
			{
				scanPoint.second.insert({ basicPoint.first, { 1, currPoint, currSide } });
			}
		}
	}
}