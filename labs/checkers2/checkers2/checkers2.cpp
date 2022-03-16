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
constexpr size_t MIN_POS = 1;
constexpr size_t MAX_POS = 8;
const std::string outputFileName = "OUTPUT.TXT";

struct Move
{
	int dx;
	int dy;

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
	Point blackPos;

	Capture(Point whitePos, Point blackPos)
		: whitePos(whitePos), blackPos(blackPos)
	{
	}
};

struct BoardCell
{
	char figure = BLANK;
	bool captured = false;
};

typedef std::vector<Capture> CaptureVec;
typedef std::array<std::array<BoardCell, MAX_POS>, MAX_POS> Board;

Point FillBoard(std::ifstream& inputFile, Board& board)
{
	Point startPoint;
	for (size_t row = 0; row < MAX_POS; ++row)
	{
		std::string line;
		std::getline(inputFile, line);

		for (size_t col = 0; col < MAX_POS; ++col)
		{
			board[row][col].figure = line[col];

			if (line[col] == CHECKER_WHITE)
			{
				startPoint = { row, col };
				board[row][col].figure = BLANK;
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

void MakeMove(const Move& move, Point& startPoint, Board& board, CaptureVec& captureVec)
{
	Point currPoint = { startPoint.x + move.dx, startPoint.y + move.dy };
	Move unitMove(move.dx / abs(move.dx), move.dy / abs(move.dy));
	if (board[currPoint.x][currPoint.y].figure == CHECKER_BLACK)
	{
		board[currPoint.x][currPoint.y].captured = true;
		if (InRange(currPoint.x + unitMove.dx, MIN_POS - 1, MAX_POS - 1) &&
			InRange(currPoint.y + unitMove.dy, MIN_POS - 1, MAX_POS - 1))
		{
			captureVec.push_back(Capture(startPoint, currPoint));
		}
	}
	Move nextMove = { 0, 0 };
	if (startPoint.x < MAX_POS - 2 && startPoint.y < MAX_POS - 2)
	{
		nextMove = { 1, 1 };
		if (unitMove == nextMove)
		{
			if (captureVec.at(captureVec.size() - 1).whitePos == startPoint)
			{
				if (board[currPoint.x][currPoint.y].figure == CHECKER_BLACK)
				{
					Point currPoint = { currPoint.x + nextMove.dx, currPoint.y + nextMove.dy };
					MakeMove({ 0, 0 }, currPoint, board, captureVec);

				}
				else
				{
					MakeMove({ 0, 0 }, currPoint, board, captureVec);
				}
			}
			else
			{
				MakeMove(move + unitMove, startPoint, board, captureVec);
			}
		}
		else
		{
			MakeMove(nextMove, startPoint, board, captureVec);
		}
	}
	if (startPoint.x > MIN_POS && startPoint.y > MIN_POS)
	{
		nextMove = { -1, -1 };
		if (unitMove == nextMove)
		{
			MakeMove(move + unitMove, startPoint, board, captureVec);
		}
		else
		{
			MakeMove(nextMove, startPoint, board, captureVec);
		}
	}
	if (startPoint.x < MAX_POS - 2 && startPoint.y > MIN_POS)
	{
		nextMove = { 1, -1 };
		if (unitMove == nextMove)
		{
			MakeMove(move + unitMove, startPoint, board, captureVec);
		}
		else
		{
			MakeMove(nextMove, startPoint, board, captureVec);
		}
	}
	if (startPoint.x > MIN_POS && startPoint.y < MAX_POS - 2)
	{
		nextMove = { -1, 1 };
		if (unitMove == nextMove)
		{
			MakeMove(move + unitMove, startPoint, board, captureVec);
		}
		else
		{
			MakeMove(nextMove, startPoint, board, captureVec);
		}
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