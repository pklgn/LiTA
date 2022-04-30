// rectangles2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

enum class PointType
{
	Begin,
	End,
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
typedef std::map<int, PointType> VertexPoints;
typedef std::map<int, VertexPoints> AxisPoints;

Dimensions GetDimensions(Point& A, Point& C);
bool ValidateFile(const std::ifstream& inputFile);
int GetAxisPerimeter(const AxisPoints& axisPoints, const Rectangles& rectangles, const ScanMode mode);
void ProcessAxisPoint(const VertexPoints& vertexPoints, AxisPoints& currAxisPoints, const Rectangles& rectangles, const ScanMode mode);

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}

	int N;
	inputFile >> N;

	Rectangles rectangles;
	AxisPoints yPoints;
	AxisPoints xPoints;

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

	std::cout << GetAxisPerimeter(yPoints, rectangles, ScanMode::Horizontally) << std::endl;
	std::cout << GetAxisPerimeter(xPoints, rectangles, ScanMode::Vertically) << std::endl;

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
int GetMultiplicity(AxisPoints& normAxisPoints);
void ProcessAxisPoint(const VertexPoints& vertexPoints, AxisPoints& currAxisPoints, 
	const Rectangles& rectangles, const ScanMode mode);

int GetAxisPerimeter(const AxisPoints& axisPoints, const Rectangles& rectangles, const ScanMode mode)
{
	int prevPos = INT_MAX;
	int axisPerimeter = 0;
	AxisPoints normAxisPoints;

	for (auto& axisPoint : axisPoints)
	{
		int axisN = GetMultiplicity(normAxisPoints);

		if (prevPos != INT_MAX)
		{
			axisPerimeter += 2 * (axisPoint.first - prevPos) * axisN;
		}

		ProcessAxisPoint(axisPoint.second, normAxisPoints, rectangles, mode);
		prevPos = axisPoint.first;
	}

	return axisPerimeter;
}

int GetMultiplicity(AxisPoints& normAxisPoints)
{
	int axisN = 0;
	int state = 0;
	for (auto& currAxisPoint : normAxisPoints)
	{
		for (auto& currVertex : currAxisPoint.second)
		{
			if (currVertex.second == PointType::Begin)
			{
				++state;
			}
			else if (currVertex.second == PointType::End)
			{
				--state;
			}
		}
		if (state == 0 && currAxisPoint.second.size() != 0)
		{
			++axisN;
		}
	}

	return axisN;
}

void ProcessAxisPoint(const VertexPoints& vertexPoints, AxisPoints& normAxisPoints, 
	const Rectangles& rectangles, const ScanMode mode)
{
	for (auto& vertexPoint : vertexPoints)
	{
		int startPos;
		int deltaPos;
		const Rectangle rectangle = rectangles[vertexPoint.first];

		switch (mode)
		{
		case ScanMode::Vertically:
			startPos = rectangle.LUVertex.x;
			deltaPos = rectangle.dimensions.width;
			break;
		case ScanMode::Horizontally:
			startPos = rectangle.LUVertex.y;
			deltaPos = -rectangle.dimensions.height;
			break;
		default:
			return;
		}

		if (vertexPoint.second == PointType::Begin)
		{
			switch (mode)
			{
			case ScanMode::Vertically:
				normAxisPoints[startPos].emplace(vertexPoint.first, PointType::Begin);
				normAxisPoints[startPos + deltaPos].emplace(vertexPoint.first, PointType::End);
				break;
			case ScanMode::Horizontally:
				normAxisPoints[startPos].emplace(vertexPoint.first, PointType::End);
				normAxisPoints[startPos + deltaPos].emplace(vertexPoint.first, PointType::Begin);
				break;
			default:
				return;
			}
		}
		else
		{
			normAxisPoints[startPos].erase(vertexPoint.first);
			normAxisPoints[startPos + deltaPos].erase(vertexPoint.first);
		}
	}
}

void ReadRectangles(std::ifstream& inputFile, Rectangles& rectangles, AxisPoints& yPoints, AxisPoints& xPoints)
{
	int N;
	inputFile >> N;

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
}