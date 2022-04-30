// rectangles2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

struct Coordinates
{
	AxisPoints yPoints;
	AxisPoints xPoints;
};

bool ValidateFile(const std::ifstream& inputFile);
int GetAxisPerimeter(const AxisPoints& axisPoints, const Rectangles& rectangles, const ScanMode mode);
void ProcessAxisPoint(const VertexPoints& vertexPoints, AxisPoints& currAxisPoints, const Rectangles& rectangles, const ScanMode mode);
void ReadRectangles(std::ifstream& inputFile, Rectangles& rectangles, Coordinates& coordinates);

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}

	Rectangles rectangles;
	Coordinates coordinates;

	ReadRectangles(inputFile, rectangles, coordinates);

	int perimeter = GetAxisPerimeter(coordinates.yPoints, rectangles, ScanMode::Horizontally) + 
		GetAxisPerimeter(coordinates.xPoints, rectangles, ScanMode::Vertically);

	std::ofstream outputFile(OUTPUT_FILE_NAME);
	outputFile << perimeter << std::endl;

	#ifdef _DEBUG
	std::cout << perimeter << std::endl;
	#endif

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

int GetMultiplicity(AxisPoints& normAxisPoints);
void ProcessAxisPoint(VertexPoints& vertexPoints, AxisPoints& currAxisPoints, 
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
	int multiplicity = 0;
	int scanState = 0;

	for (auto& normAxisPoint : normAxisPoints)
	{
		for (auto& vertex : normAxisPoint.second)
		{
			if (vertex.second == PointType::Begin)
			{
				++scanState;
			}
			else if (vertex.second == PointType::End)
			{
				--scanState;
			}
		}
		if (scanState == 0 && normAxisPoint.second.size() != 0)
		{
			++multiplicity;
		}
	}

	return multiplicity;
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

	return;
}

Dimensions GetDimensions(Point& A, Point& C);
void ReadRectangles(std::ifstream& inputFile, Rectangles& rectangles, Coordinates& coordinates)
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

		coordinates.yPoints[A.x].emplace(rectangleId, PointType::Begin);
		coordinates.yPoints[B.x].emplace(rectangleId, PointType::End);
		coordinates.xPoints[C.y].emplace(rectangleId, PointType::Begin);
		coordinates.xPoints[B.y].emplace(rectangleId, PointType::End);
	}

	return;
}

Dimensions GetDimensions(Point& A, Point& C)
{
	Dimensions result;
	result.width = C.x - A.x;
	result.height = A.y - C.y;

	return result;
}