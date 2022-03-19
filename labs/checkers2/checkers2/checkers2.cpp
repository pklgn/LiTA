/*
* 1.8. Шашки 2 (7) 
* На шахматном поле расположены N черных шашек и одна белая дамка.
* Требуется написать программу, которая по заданному расположению шашек определяет,
* какое максимальное количество шашек может взять белая дамка за один ход.
* 
* Стандарт C++ 17
* Среда разработки: Visual Studio 2019
* ОС: Windows 10
* 
* Выполнил: Ермаков Павел, ПС-21
*/

#include <iostream>
#include <stack>
#include <optional>
#include <string>
#include <fstream>
#include <array>
#include <vector>

constexpr char BLANK = '0';
constexpr char CHECKER_BLACK = '1';
constexpr char CHECKER_WHITE = '2';
constexpr short COORDINATE_SHIFT = 1;
constexpr size_t MIN_POS = 1;
constexpr size_t MAX_POS = 8;
const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

struct Move
{
	int dx;
	int dy;

	Move(int dx = 0, int dy = 0)
		: dx(dx), dy(dy)
	{
	}

	Move operator + (const Move& m) const
	{
		return Move(m.dx + this->dx, m.dy + this->dy);
	}

	bool operator == (const Move& move) const
	{
		return this->dx == move.dx && this->dy == move.dy;
	}
};

const Move OFFSET_DR = { 1, 1 };
const Move OFFSET_DL = { 1, -1 };
const Move OFFSET_UR = { -1, 1 };
const Move OFFSET_UL = { -1, -1 };
const Move NO_MOVE = { 0, 0 };

struct Point
{
	size_t x = 0;
	size_t y = 0;

	bool operator == (const Point& point) const
	{
		return this->x == point.x && this->y == point.y;
	}
};

struct Capture
{
	Point whitePos;
	Point afterPos;

	Capture(Point whitePos, Point afterPos)
		: whitePos(whitePos), afterPos(afterPos)
	{
	}
};

typedef std::vector<Capture> CaptureVec;
typedef std::array<std::array<char, MAX_POS>, MAX_POS> Board;

Point FillBoard(std::ifstream& inputFile, Board& board)
{
	Point startPoint;
	for (size_t row = 0; row < MAX_POS; ++row)
	{
		std::string line;
		std::getline(inputFile, line);

		for (size_t col = 0; col < MAX_POS; ++col)
		{
			board[row][col] = line[col];

			if (line[col] == CHECKER_WHITE)
			{
				startPoint = { row, col };
				board[row][col] = BLANK;
			}
		}
	}

	return startPoint;
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

template<typename T>
bool InRange(T value, T min, T max)
{
	return value >= min && value <= max;
}

CaptureVec MakeMove(const Move& move, const Move& moveDirect, const Point& startPoint, Board& board, CaptureVec& captureVec);
CaptureVec MakeCapture(const Move& prevMoveDirect, const Point& startPoint, Board board, CaptureVec captureVec)
{
	CaptureVec interResult;
	CaptureVec result = captureVec;
	if (startPoint.x < MAX_POS - COORDINATE_SHIFT && 
		startPoint.y < MAX_POS - COORDINATE_SHIFT)
	{
		if (prevMoveDirect.dx != -OFFSET_DR.dx || prevMoveDirect.dy != -OFFSET_DR.dy)
		{
			Board boardDR = board;
			CaptureVec captureVecDR = captureVec;
			interResult = MakeMove(OFFSET_DR, OFFSET_DR, startPoint, boardDR, captureVecDR);
			if (interResult.size() > result.size())
			{
				result.clear();
				result.insert(result.begin(), interResult.begin(), interResult.end());
			}
		}
	}
	if (startPoint.x > MIN_POS - COORDINATE_SHIFT && 
		startPoint.y > MIN_POS - COORDINATE_SHIFT)
	{
		if (prevMoveDirect.dx != -OFFSET_UL.dx || prevMoveDirect.dy != -OFFSET_UL.dy)
		{
			Board boardUL = board;
			CaptureVec captureVecUL = captureVec;
			interResult = MakeMove(OFFSET_UL, OFFSET_UL, startPoint, boardUL, captureVecUL);
			if (interResult.size() > result.size())
			{
				result.clear();
				result.insert(result.begin(), interResult.begin(), interResult.end());
			}
		}
	}
	if (startPoint.x < MAX_POS - COORDINATE_SHIFT && 
		startPoint.y > MIN_POS - COORDINATE_SHIFT)
	{
		if (prevMoveDirect.dx != -OFFSET_DL.dx || prevMoveDirect.dy != -OFFSET_DL.dy)
		{
			Board boardDL = board;
			CaptureVec captureVecDL = captureVec;
			interResult = MakeMove(OFFSET_DL, OFFSET_DL, startPoint, boardDL, captureVecDL);
			if (interResult.size() > result.size())
			{
				result.clear();
				result.insert(result.begin(), interResult.begin(), interResult.end());
			}
		}
	}
	if (startPoint.x > MIN_POS - COORDINATE_SHIFT && 
		startPoint.y < MAX_POS - COORDINATE_SHIFT)
	{
		if (prevMoveDirect.dx != -OFFSET_UR.dx || prevMoveDirect.dy != -OFFSET_UR.dy)
		{
			Board boardUR = board;
			CaptureVec captureVecUR = captureVec;
			interResult = MakeMove(OFFSET_UR, OFFSET_UR, startPoint, boardUR, captureVecUR);
			if (interResult.size() > result.size())
			{
				result.clear();
				result.insert(result.begin(), interResult.begin(), interResult.end());
			}
		}
	}

	return result;
}

CaptureVec MakeMove(const Move& move, const Move& moveDirect, const Point& startPoint, Board& board, CaptureVec& captureVec)
{
	Point currPoint = { startPoint.x + move.dx, startPoint.y + move.dy };
	if (board[currPoint.x][currPoint.y] == CHECKER_BLACK && 
		board[currPoint.x + moveDirect.dx][currPoint.y + moveDirect.dy] != CHECKER_BLACK &&
		InRange(currPoint.x, MIN_POS, MAX_POS - 2) && InRange(currPoint.y, MIN_POS, MAX_POS - 2))
	{
		Point afterPoint = { currPoint.x + moveDirect.dx, currPoint.y + moveDirect.dy };
		CaptureVec result;
		Capture capture = { startPoint, afterPoint };
		captureVec.push_back(capture);
		board[currPoint.x][currPoint.y] = BLANK;

		while (InRange(currPoint.x, MIN_POS - COORDINATE_SHIFT, MAX_POS - COORDINATE_SHIFT) && 
			InRange(currPoint.y, MIN_POS - COORDINATE_SHIFT, MAX_POS - COORDINATE_SHIFT) &&
			board[currPoint.x][currPoint.y] != CHECKER_BLACK)
		{
			Point nextPoint = { currPoint.x + moveDirect.dx, currPoint.y + moveDirect.dy };
			CaptureVec interResult = MakeCapture(moveDirect, nextPoint, board, captureVec);
			if (interResult.size() > result.size())
			{
				result.clear();
				result.insert(result.begin(), interResult.begin(), interResult.end());
			}

			currPoint.x += moveDirect.dx;
			currPoint.y += moveDirect.dy;
		}

		return result;
	}
	else
	{
		if (board[currPoint.x][currPoint.y] == CHECKER_BLACK &&
			board[currPoint.x + moveDirect.dx][currPoint.y + moveDirect.dy] == CHECKER_BLACK )
		{
			return captureVec;
		}
		else if (InRange(currPoint.x, MIN_POS, MAX_POS - 2) && InRange(currPoint.y, MIN_POS, MAX_POS - 2))
		{
			CaptureVec result = MakeMove(move + moveDirect, moveDirect, startPoint, board, captureVec);

			return result;
		}
		else
		{
			return captureVec;
		}
	}
}

bool PrintCaptureVec(const CaptureVec& resultVec, std::ofstream& outputFile)
{
	outputFile << resultVec.size() << std::endl;

	for (size_t i = 0; i < resultVec.size(); i++)
	{
		outputFile << resultVec.at(i).whitePos.x + COORDINATE_SHIFT 
			<< " " << resultVec.at(i).whitePos.y + COORDINATE_SHIFT << std::endl;
		if (i == resultVec.size() - 1)
		{
			outputFile << resultVec.at(i).afterPos.x + COORDINATE_SHIFT 
				<< " " << resultVec.at(i).afterPos.y + COORDINATE_SHIFT << std::endl;
		}
	}

	if (!outputFile.flush())
	{
		std::cout << "Can't save data on disk\n";

		return false;
	}

	return true;
}

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}

	Board board;
	Point startPoint = FillBoard(inputFile, board);
	CaptureVec captureVec;
	CaptureVec resultVec = MakeCapture(NO_MOVE, startPoint, board, captureVec);
	
	std::ofstream outputFile(OUTPUT_FILE_NAME);
	status = PrintCaptureVec(resultVec, outputFile);
	if (!status)
	{
		return 1;
	}

	return 0;
}