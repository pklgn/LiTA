#include <iostream>
#include <stack>
#include <optional>
#include <string>
#include <fstream>
#include <array>
#include <vector>

constexpr short BLANK = '0';
constexpr short CHECKER_BLACK = '1';
constexpr short CHECKER_WHITE = '2';
constexpr size_t MIN_POS = 0;
constexpr size_t MAX_POS = 8;
const std::string outputFileName = "OUTPUT.TXT";

struct Move
{
	short dx;
	short dy;

	Move(short dx = 0, short dy = 0)
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

struct Point
{
	size_t x;
	size_t y;

	bool operator == (const Point& point) const
	{
		return this->x == point.x && this->y == point.y;
	}
};

struct Capture
{
	Point whitePos;
	Point blackPos;

	Capture(Point whitePos, Point blackPos)
		: whitePos(whitePos), blackPos(blackPos)
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
	return value > min && value < max;
}

void MakeMove(Move& move, Point& startPoint, Board& board, CaptureVec captureVec)
{
	Point currPoint = { startPoint.x + move.dx, startPoint.y + move.dy };
	if (board[currPoint.x][currPoint.y] == CHECKER_WHITE)
	{
		Move captureMove(move.dx / abs(move.dx), move.dy / abs(move.dy));
		if (InRange(currPoint.x + captureMove.dx, MIN_POS, MAX_POS - 1) && 
			InRange(currPoint.y + captureMove.dy, MIN_POS, MAX_POS - 1))
		{
			Move nextMove = captureMove + move;
			MakeMove(nextMove, startPoint, board, captureVec);
			captureVec.push_back(Capture(startPoint, currPoint));
		}
		else
		{
			return;
		}
	}
	Move nextMove = { 0, 0 };
	if (startPoint.x < MAX_POS - 2 && startPoint.y < MAX_POS - 2)
	{
		CaptureVec captureVecDR = captureVec;
		nextMove = { 1, 1 };
		MakeMove(move, currPoint, board, captureVecDR);
	}
	if (startPoint.x > MIN_POS && startPoint.y > MIN_POS)
	{
		CaptureVec captureVecUL = captureVec;
		nextMove = { -1, -1 };
		MakeMove(move, currPoint, board, captureVecUL);
	}
	if (startPoint.x < MAX_POS - 2 && startPoint.y > MIN_POS)
	{
		CaptureVec captureVecDL = captureVec;
		nextMove = { 1, -1 };
		MakeMove(move, currPoint, board, captureVecDL);
	}
	if (startPoint.x > MIN_POS && startPoint.y < MAX_POS - 2)
	{
		CaptureVec captureVecUR = captureVec;
		nextMove = { -1, 1 };
		MakeMove(move, currPoint, board, captureVecUR);
	}
	if (nextMove.dx == 0 || nextMove.dy == 0)
	{
		return;
	}
}

int main(int argc, char* argv[])
{
	std::ifstream inputFile(argv[1]);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}
	Board board;
	Point startPoint = FillBoard(inputFile, board);
	CaptureVec captureVec;
	return 0;
}