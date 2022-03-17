#include <iostream>
#include <stack>
#include <optional>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <cmath>

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

void MakeMove(const Move& move, Point& startPoint, Board& board, CaptureVec& captureVec, CaptureVec& resultVec)
{
 	Point currPoint = { startPoint.x + move.dx, startPoint.y + move.dy };
	Move unitMove;
	if (move.dx == 0)
	{
		unitMove = move;
	}
	else
	{
		int dx = move.dx / std::abs(move.dx);
		int dy = move.dy / std::abs(move.dy);
		unitMove = Move(dx, dy);
	}
	if (board[currPoint.x][currPoint.y].figure == CHECKER_BLACK)
	{
		if (InRange(currPoint.x + unitMove.dx, MIN_POS, MAX_POS - 2) &&
			InRange(currPoint.y + unitMove.dy, MIN_POS, MAX_POS - 2))
		{
			board[currPoint.x][currPoint.y].captured = true;
			board[currPoint.x][currPoint.y].figure = BLANK;
			captureVec.push_back(Capture(startPoint, currPoint));
		}
		else
		{
			return;
		}
	}
	Move nextMove = { 0, 0 };
	if (currPoint.x < MAX_POS - 1 && currPoint.y < MAX_POS - 1)
	{
		nextMove = { 1, 1 };
		if (nextMove.dx != -unitMove.dx || nextMove.dy != -unitMove.dy)
		{
			if (unitMove == nextMove)
			{
				if (captureVec.at(captureVec.size() - 1).whitePos == startPoint)
				{
					if (board[currPoint.x][currPoint.y].figure == CHECKER_BLACK)
					{
						Point nextPoint = { currPoint.x + nextMove.dx, currPoint.y + nextMove.dy };
						MakeMove({ 0, 0 }, nextPoint, board, captureVec, resultVec);

					}
					else
					{
						MakeMove({ 0, 0 }, currPoint, board, captureVec, resultVec);
					}
				}
				else
				{
					MakeMove(move + unitMove, startPoint, board, captureVec, resultVec);
				}
			}
			else
			{
				MakeMove(nextMove, currPoint, board, captureVec, resultVec);
			}
		}
	}
	if (currPoint.x > MIN_POS - 1 && currPoint.y > MIN_POS - 1)
	{
		nextMove = { -1, -1 };
		if (nextMove.dx != -unitMove.dx || nextMove.dy != -unitMove.dy)
		{
			if (unitMove == nextMove)
			{
				if (captureVec.at(captureVec.size() - 1).whitePos == startPoint)
				{
					if (board[currPoint.x][currPoint.y].figure == CHECKER_BLACK)
					{
						Point nextPoint = { currPoint.x + nextMove.dx, currPoint.y + nextMove.dy };
						MakeMove({ 0, 0 }, nextPoint, board, captureVec, resultVec);

					}
					else
					{
						MakeMove({ 0, 0 }, currPoint, board, captureVec, resultVec);
					}
				}
				else
				{
					MakeMove(move + unitMove, startPoint, board, captureVec, resultVec);
				}
			}
			else
			{
				MakeMove(nextMove, currPoint, board, captureVec, resultVec);
			}
		}
	}
	if (currPoint.x < MAX_POS - 1 && currPoint.y > MIN_POS - 1)
	{
		nextMove = { 1, -1 };
		if (nextMove.dx != -unitMove.dx || nextMove.dy != -unitMove.dy)
		{
			if (unitMove == nextMove)
			{
				if (captureVec.at(captureVec.size() - 1).whitePos == startPoint)
				{
					if (board[currPoint.x][currPoint.y].figure == CHECKER_BLACK)
					{
						Point nextPoint = { currPoint.x + nextMove.dx, currPoint.y + nextMove.dy };
						MakeMove({ 0, 0 }, nextPoint, board, captureVec, resultVec);

					}
					else
					{
						MakeMove({ 0, 0 }, currPoint, board, captureVec, resultVec);
					}
				}
				else
				{
					MakeMove(move + unitMove, startPoint, board, captureVec, resultVec);
				}
			}
			else
			{
				MakeMove(nextMove, currPoint, board, captureVec, resultVec);
			}
		}
	}
	if (currPoint.x > MIN_POS - 1 && currPoint.y < MAX_POS - 1)
	{
		nextMove = { -1, 1 };
		if (nextMove.dx != -unitMove.dx || nextMove.dy != -unitMove.dy)
		{
			if (unitMove == nextMove)
			{
				if (captureVec.at(captureVec.size() - 1).whitePos == startPoint)
				{
					if (board[currPoint.x][currPoint.y].figure == CHECKER_BLACK)
					{
						Point nextPoint = { currPoint.x + nextMove.dx, currPoint.y + nextMove.dy };
						MakeMove({ 0, 0 }, nextPoint, board, captureVec, resultVec);

					}
					else
					{
						MakeMove({ 0, 0 }, currPoint, board, captureVec, resultVec);
					}
				}
				else
				{
					MakeMove(move + unitMove, startPoint, board, captureVec, resultVec);
				}
			}
			else
			{
				MakeMove(nextMove, currPoint, board, captureVec, resultVec);
			}
		}
	}
	if (nextMove.dx == 0 || nextMove.dy == 0)
	{
		if (captureVec.size() > resultVec.size())
		{
			resultVec = captureVec;
		}

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
	CaptureVec resultVec;
	MakeMove({ 0, 0 }, startPoint, board, captureVec, resultVec);

	return 0;
}