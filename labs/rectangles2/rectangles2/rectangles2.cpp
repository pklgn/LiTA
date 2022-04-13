// rectangles2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

typedef std::pair<int, int> RectangleSide;

enum class PointType
{
	Begin,
	End,
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

struct SidePoint
{
	int pos;
	PointType type;
};

struct Dimensions
{
	int width;
	int height;
};

struct RectangleSideEvent
{
	RectangleSide side;
	int event;
};

typedef std::map<int, std::vector<SidePoint>> SidePoints;

Dimensions GetDimensions(Point& A, Point& C);
bool ValidateFile(const std::ifstream& inputFile);
void PointsSort(std::vector<SidePoint>& sidePoints);

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

	SidePoints xPoints;
	SidePoints yPoints;

	for (int i = 0; i < N; ++i)
	{
		Point A;
		Point C;
		inputFile >> A >> C;
		Dimensions dimensions = GetDimensions(A, C);
		Point B = { A.x + dimensions.width, A.y };
		Point D = { A.x, A.y + dimensions.height };
		xPoints[A.y].push_back({ A.x, PointType::Begin });
		xPoints[A.y].push_back({ B.x, PointType::End });
		xPoints[C.y].push_back({ C.x, PointType::End });
		xPoints[C.y].push_back({ D.x, PointType::Begin });
		yPoints[A.x].push_back({ A.y, PointType::End });
		yPoints[B.x].push_back({ B.y, PointType::End });
		yPoints[C.x].push_back({ C.y, PointType::Begin });
		yPoints[D.x].push_back({ D.y, PointType::Begin });
	}

	//Insertion sort
	for (auto& points : xPoints)
	{
		PointsSort(points.second);
	}
	for (auto& points : yPoints)
	{
		PointsSort(points.second);
	}

	for (auto& xPoint: yPoints)
	{
		for (auto& yPoint: xPoints)
		{
			std::cout << xPoint.first << " " << yPoint.first << std::endl;
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

void PointsSort(std::vector<SidePoint>& sidePoints)
{
	for (int i = 1; i < sidePoints.size(); ++i)
	{
		for (int j = i; j > 0 && sidePoints[j - 1].pos > sidePoints[j].pos; --j)
		{
			std::swap(sidePoints[j - 1], sidePoints[j]);
		}
	}
}