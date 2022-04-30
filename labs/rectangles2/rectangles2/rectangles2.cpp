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
};

enum class SideType
{
	Left,
	Up,
	Right,
	Down,
};

enum class ScanMode
{
	Vertically,
	Horizontally
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

typedef std::vector<Rectangle> Rectangles;
typedef std::map<int, std::map<int, PointType>> EventPointsMap;

Dimensions GetDimensions(Point& A, Point& C);
bool ValidateFile(const std::ifstream& inputFile);
int GetAxisPerimeter(EventPointsMap& axisPoints, Rectangles& rectangles, ScanMode mode);

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

	Rectangles rectangles;
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
		
		yPoints[A.x].emplace(rectangleId, PointType::Begin);
		yPoints[B.x].emplace(rectangleId, PointType::End);
		xPoints[C.y].emplace(rectangleId, PointType::Begin);
		xPoints[B.y].emplace(rectangleId, PointType::End);
	}

	std::cout << GetAxisPerimeter(xPoints, rectangles, ScanMode::Vertically) + GetAxisPerimeter(yPoints, rectangles, ScanMode::Horizontally) << std::endl;

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
	result.height = A.y - C.y;

	return result;
}

int GetAxisPerimeter(EventPointsMap& axisPoints, Rectangles& rectangles, ScanMode mode)
{
	EventPointsMap currAxisPoints;
	int axisPerimeter = 0;
	int prevPos = INT_MAX;
	for (auto& axisPoint : axisPoints)
	{
		int axisN = 0;
		int state = 0;
		for (auto& currXPoint : currAxisPoints)
		{
			for (auto& currVertex : currXPoint.second)
			{
				if (currVertex.second == PointType::Begin)
				{
					++state;
				}
				else
				{
					--state;
				}
				if (state == 0)
				{
					++axisN;
				}
			}
		}
		if (prevPos != INT_MAX)
		{
			axisPerimeter += 2 * (axisPoint.first - prevPos) * axisN;
		}

		for (auto& vertex : axisPoint.second)
		{
			const Rectangle rectangle = rectangles[vertex.first];
			int LUVertexPos = mode == ScanMode::Horizontally ? rectangle.LUVertex.y : rectangle.LUVertex.x;
			int deltaPos = mode == ScanMode::Horizontally ? rectangle.dimensions.height : rectangle.dimensions.width;

			if (vertex.second == PointType::Begin)
			{
				currAxisPoints[LUVertexPos].emplace(vertex.first, PointType::Begin);
				currAxisPoints[LUVertexPos + deltaPos].emplace(vertex.first, PointType::End);
			}
			else
			{
				currAxisPoints[LUVertexPos].erase(vertex.first);
				currAxisPoints[LUVertexPos - deltaPos].erase(vertex.first);
			}
		}
		prevPos = axisPoint.first;
	}

	return axisPerimeter;
}