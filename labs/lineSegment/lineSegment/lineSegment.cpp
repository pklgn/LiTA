/* 11.8. Пересечение отрезков (5)
* Два отрезка на плоскости заданы целочисленными координатами своих концов в декартовой системе координат.
* Требуется определить, существует ли у них общая точка.
*
* 
* Стандарт C++ 17
* Среда разработки: Visual Studio 2019, MSVC
* ОС: Windows 10
*
* Выполнил: Ермаков Павел, ПС-21
*/

#include <limits>
#include <fstream>
#include <iostream>
#include <optional>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

constexpr int NO_POSITION = 0;
constexpr long double ANY_VALUE = std::numeric_limits<long double>::infinity();

struct Point
{
	long double x;
	long double y;

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

typedef std::optional<long double> Position;

bool ValidateFile(const std::ifstream& inputFile);
bool LineEquation(Point const& A, Point const& B, long double x, Point& result);
Position GetXIntersectionPoint(LineSegment const& firstLineSegment, LineSegment const& secondLineSegment);
bool BelongLineSegment(Point const& inspectedPoint, Point const& firstBorder, Point const& secondBorder);
bool CalculateIntersectionPoint(LineSegment const& firstLineSegment, LineSegment const& secondLineSegment, Point& result);

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

	bool intersectionExist = false;
	Point result{ NO_POSITION, NO_POSITION };
	std::ofstream outputFile(OUTPUT_FILE_NAME);
	if (A.x == B.x)
	{
		result.x = A.x;
		intersectionExist = CalculateIntersectionPoint(firstLineSegment, secondLineSegment, result);
	}
	else if (C.x == D.x)
	{
		result.x = C.x;
		intersectionExist = CalculateIntersectionPoint(firstLineSegment, secondLineSegment, result);
	}
	else
	{
		Position xIntersection = GetXIntersectionPoint(firstLineSegment, secondLineSegment);

		if (!xIntersection.has_value())
		{
			outputFile << "No" << std::endl;

			return 0;
		}

		result.x = xIntersection.value();
		intersectionExist = CalculateIntersectionPoint(firstLineSegment, secondLineSegment, result);
	}

	if (intersectionExist &&
		BelongLineSegment(result, A, B) &&
		BelongLineSegment(result, C, D))
	{
		outputFile << "Yes" << std::endl;

		return 0;
	}

	outputFile << "No" << std::endl;

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
	long double numerator = firstLineSegment.A.x * (firstLineSegment.A.y - firstLineSegment.B.y) / (firstLineSegment.A.x - firstLineSegment.B.x) - secondLineSegment.A.x * (secondLineSegment.A.y - secondLineSegment.B.y) / (secondLineSegment.A.x - secondLineSegment.B.x) - firstLineSegment.A.y + secondLineSegment.A.y;
	long double denominator = (firstLineSegment.A.y - firstLineSegment.B.y) / (firstLineSegment.A.x - firstLineSegment.B.x) - (secondLineSegment.A.y - secondLineSegment.B.y) / (secondLineSegment.A.x - secondLineSegment.B.x);

	if (numerator == 0 && numerator == denominator)
	{
		bool among = BelongLineSegment(secondLineSegment.A, firstLineSegment.A, firstLineSegment.B);
		if (among)
		{
			return secondLineSegment.A.x;
		}
		among = BelongLineSegment(secondLineSegment.B, firstLineSegment.A, firstLineSegment.B);
		if (among)
		{
			return secondLineSegment.B.x;
		}

		return firstLineSegment.A.x;

	}

	if (denominator == 0)
	{
		return std::nullopt;
	}

	return numerator / denominator;
}

bool LineEquation(Point const& A, Point const& B, long double x, Point& result)
{
	if (B.x == A.x && B.x == x)
	{
		result.y = ANY_VALUE;

		return true;
	}

	if (B.x == A.x)
	{
		return false;
	}

	result.y = (x - A.x) * (B.y - A.y) / (B.x - A.x) + A.y;

	return true;
}

bool BelongLineSegment(Point const& inspectedPoint, Point const& firstBorder, Point const& secondBorder)
{
	return (inspectedPoint.x >= std::min(firstBorder.x, secondBorder.x) &&
			inspectedPoint.x <= std::max(firstBorder.x, secondBorder.x) &&
			inspectedPoint.y >= std::min(firstBorder.y, secondBorder.y) &&
			inspectedPoint.y <= std::max(firstBorder.y, secondBorder.y));
}

bool CalculateIntersectionPoint(LineSegment const& firstLineSegment, LineSegment const& secondLineSegment, Point& result)
{
	bool isComputable = LineEquation(firstLineSegment.A, firstLineSegment.B, result.x, result);
	if (!isComputable)
	{
		return false;
	}

	if (result.y == ANY_VALUE)
	{
		isComputable = LineEquation(secondLineSegment.A, secondLineSegment.B, result.x, result);
		if (!isComputable)
		{
			return false;
		}

		if (result.y == ANY_VALUE)
		{
			bool among = BelongLineSegment(secondLineSegment.A, firstLineSegment.A, firstLineSegment.B);
			if (among)
			{
				result.y = secondLineSegment.A.y;

				return true;
			}
			among = BelongLineSegment(secondLineSegment.B, firstLineSegment.A, firstLineSegment.B);
			if (among)
			{
				result.y = secondLineSegment.B.y;
				
				return true;
			}

			result.y = secondLineSegment.A.y;
		}
	}

	return true;
}