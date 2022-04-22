// rectangles2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

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
		/*
		yPoints[A.y].insert({ A.x, { PointType::Begin, SideType::Up } });
		yPoints[A.y].insert({ B.x, { PointType::End, SideType::Up } });
		yPoints[C.y].insert({ C.x, { PointType::End, SideType::Down } });
		yPoints[C.y].insert({ D.x, { PointType::Begin, SideType::Down } });
		xPoints[A.x].insert({ A.y, { PointType::End, SideType::Left } });
		xPoints[B.x].insert({ B.y, { PointType::End, SideType::Right } });
		xPoints[C.x].insert({ C.y, { PointType::Begin, SideType::Right } });
		xPoints[D.x].insert({ D.y, { PointType::Begin, SideType::Left } });
		*/

		InsertVertexPoint(A.y, yPoints, A.x, { PointType::Begin, SideType::Up }, xPoints);
		InsertVertexPoint(A.y, yPoints, B.x, { PointType::End, SideType::Up }, xPoints);
		InsertVertexPoint(C.y, yPoints, C.x, { PointType::End, SideType::Down }, xPoints);
		InsertVertexPoint(C.y, yPoints, D.x, { PointType::Begin, SideType::Down }, xPoints);
		InsertVertexPoint(A.x, xPoints, A.y, { PointType::End, SideType::Left }, yPoints);
		InsertVertexPoint(B.x, xPoints, B.y, { PointType::End, SideType::Right }, yPoints);
		InsertVertexPoint(C.x, xPoints, C.y, { PointType::Begin, SideType::Right }, yPoints);
		InsertVertexPoint(D.x, xPoints, D.y, { PointType::Begin, SideType::Left }, yPoints);
	}
	
	for (auto& xPoint: xPoints)
	{
		int state = 0;
		SideType currSide;
		PointType currPoint;
		for (auto& yPoint: yPoints)
		{ 
			if (state > 0)
			{
				yPoint.second.insert({ xPoint.first, { currPoint, currSide } });
			}
			else if (state < 0)
			{
				yPoint.second.insert({ xPoint.first, { currPoint, currSide } });
			}
			if (xPoint.second.count(yPoint.first))
			{
				if (xPoint.second[yPoint.first].pointType == PointType::Begin)
				{
					state += 1;
					
				}
				else if (xPoint.second[yPoint.first].pointType == PointType::End)
				{
					state -= 1;
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
		}
	}

	for (auto& yPoint : yPoints)
	{
		int state = 0;
		SideType currSide;
		PointType currPoint;
		for (auto& xPoint : xPoints)
		{
			if (state > 0)
			{
				xPoint.second.insert({ yPoint.first, { currPoint, currSide } });
			}
			else if (state < 0)
			{
				xPoint.second.insert({ yPoint.first, { currPoint, currSide } });
			}
			if (yPoint.second.count(xPoint.first))
			{
				if (yPoint.second[xPoint.first].pointType == PointType::Begin)
				{
					state += 1;
				}
				else if (yPoint.second[xPoint.first].pointType == PointType::End)
				{
					state -= 1;
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
		}
	}
	int xResult = 0;

	std::pair<int, VertexInfo> lastSafePoint;
	for (auto& xPoint: xPoints)
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
			if (!intersectionState)
			{
				if (point.second.pointType == PointType::Begin)
				{
					if (sideState == 0)
					{
						lastSafePoint = point;
					}
					sideState += 1;
				}
				else if (point.second.pointType == PointType::End)
				{
					if (sideState > 1)
					{
						lastSafePoint = point;
					}
					sideState -= 1;
					xResult += point.first - lastSafePoint.first;
				}
			}
			else
			{
				if (point.second.pointType == PointType::Begin)
				{
					sideState += 1;
				}
				else if (point.second.pointType == PointType::End)
				{
					sideState -= 1;
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
			if (!intersectionState)
			{
				if (point.second.pointType == PointType::Begin)
				{
					if (sideState == 0)
					{
						lastSafePoint = point;
					}
					sideState += 1;
				}
				else if (point.second.pointType == PointType::End)
				{
					if (sideState > 1)
					{
						lastSafePoint = point;
					}
					sideState -= 1;
					yResult += point.first - lastSafePoint.first;
				}
			}
			else
			{
				if (point.second.pointType == PointType::Begin)
				{
					sideState += 1;
				}
				else if (point.second.pointType == PointType::End)
				{
					sideState -= 1;
				}
			}
		}
	}

	int result = xResult + yResult;

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
	if (axisPoints[axis].count(axisPosition) && 
		axisPoints[axis][axisPosition].pointType != vertexInfo.pointType)
	{
		axisPoints[axis].erase(axisPosition);
		normalAxisPoints[axisPosition].erase(axis);
	}
	else
	{
		axisPoints[axis].insert({ axisPosition, vertexInfo });
	}
}