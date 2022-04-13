// rectangles2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

typedef std::pair<int, int> RectangleSide;


struct Point
{
	int x;
	int y;

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

struct RectangleSideEvent
{
	RectangleSide side;
	int event;
};

typedef std::map<int, std::vector<RectangleSideEvent>> RectangleSides;


Dimensions GetDimensions(Point& A, Point& C);
bool ValidateFile(const std::ifstream& inputFile);


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

	RectangleSides xSides;
	RectangleSides ySides;

	for (int i = 0; i < N; ++i)
	{
		Point A;
		Point C;
		inputFile >> A >> C;
		Dimensions dimensions = GetDimensions(A, C);
		Point B = { A.x + dimensions.width, A.y };
		Point D = { A.x, A.y + dimensions.height };
		RectangleSideEvent uSideEvent = { { A.x, B.x }, 1 };
		RectangleSideEvent rSideEvent = { { B.y, C.y }, -1 };
		RectangleSideEvent dSideEvent = { { D.x, C.x }, -1 };
		RectangleSideEvent lSideEvent = { { A.y, D.y }, 1 };
		xSides[A.y].push_back(uSideEvent);
		xSides[C.y].push_back(dSideEvent);
		ySides[A.x].push_back(lSideEvent);
		ySides[C.x].push_back(rSideEvent);
	}

	for (auto& ySide: ySides)
	{
		int y = ySide.first;
		for (RectangleSides::iterator i = xSides.begin(); i != xSides.end(); i++)
		{
			if (i->first == y)
			{
			}
		}

		for (auto& side: ySide.second)
		{
			
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