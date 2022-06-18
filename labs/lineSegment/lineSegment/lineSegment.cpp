// lineSegment.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <iostream>
#include <optional>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

constexpr int NO_POSITION = 0;

struct Point
{
	double x;
	double y;

	friend std::ifstream& operator>>(std::ifstream& is, Point& point)
	{
		is >> point.x >> point.y;

		return is;
	}
};

struct LineSegment
{
	Point A;
	Point B;
};

typedef std::optional<double> Position;

bool ValidateFile(const std::ifstream& inputFile);
bool LineEquation(Point const& A, Point const& B, double x, Point& result);

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}

	Point A;
	Point B;
	Point C;
	Point D;

	inputFile >> A >> B >> C >> D;

	LineSegment firstLineSegment{ A, B };
	LineSegment secondLineSegment{ C, D };

	Position xIntersection = GetXIntersectionPoint(firstLineSegment, secondLineSegment);
	
	if (!xIntersection.has_value())
	{
		std::cout << "No" << std::endl;

		return 0;
	}

	Point result{ xIntersection.value(), NO_POSITION };
	bool isComputable;
	if (!isComputable)
	{
		std::cout << "No" << std::endl;

		return 0;
	}

	if (result.y == NAN)
	{
		bool isComputable = LineEquation(secondLineSegment.A, secondLineSegment.B, result.x, result);
		if (!isComputable && result.y == NAN)
		{
			std::cout << "No" << std::endl;

			return 0;
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

Position GetXIntersectionPoint(LineSegment const& firstLineSegment, LineSegment const& secondLineSegment)
{
	int rightSide = firstLineSegment.A.x * (firstLineSegment.B.y - firstLineSegment.A.y) -
					secondLineSegment.A.x * (secondLineSegment.B.y - secondLineSegment.A.y) +
					secondLineSegment.A.y * (secondLineSegment.B.x - secondLineSegment.A.x) -
					firstLineSegment.A.y * (firstLineSegment.B.x - firstLineSegment.A.x);
	int leftSide = (firstLineSegment.B.y - firstLineSegment.A.y) - (secondLineSegment.B.y - secondLineSegment.A.y);

	if (leftSide == 0)
	{
		return std::nullopt;
	}

	return rightSide / leftSide;
}

bool LineEquation(Point const& A, Point const& B, double x, Point& result)
{
	if (B.x == A.x && B.x == x)
	{
		result.y = NAN;

		return true;
	}

	if (B.x == A.x)
	{
		return false;
	}

	result.y = (x - A.x) * (B.y - A.y) / (B.x - A.x) + A.y;

	return true;
}

void CalculateLineSegmentsIntersection(LineSegment const& firstLineSegment, LineSegment const& secondLineSegment)
{
	
}