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

struct MoveNode {
	struct Move move;
	struct MoveNode* firstchild;
	struct MoveNode* nextsibling;
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
CaptureVec MakeMove(const Move& move, const Move& moveDirect, Point& startPoint, Board& board, CaptureVec& captureVec);
CaptureVec MakeCapture(const Move& prevMoveDirect, Point& startPoint, Board board, CaptureVec captureVec)
{
	CaptureVec interResult;
	CaptureVec result = captureVec;
	if (startPoint.x < MAX_POS - 1 && startPoint.y < MAX_POS - 1)
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
	if (startPoint.x > MIN_POS - 1 && startPoint.y > MIN_POS - 1)
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
	if (startPoint.x < MAX_POS - 1 && startPoint.y > MIN_POS - 1)
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
	if (startPoint.x > MIN_POS - 1 && startPoint.y < MAX_POS - 1)
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
	if (result.size() > 0)
	{
		return result;
	}
	else
	{
		return captureVec;
	}
	//return result.size() > 0 ? result : captureVec;
}

CaptureVec MakeMove(const Move& move, const Move& moveDirect, Point& startPoint, Board& board, CaptureVec& captureVec)
{
	Point currPoint = { startPoint.x + move.dx, startPoint.y + move.dy };
	if (board[currPoint.x][currPoint.y].figure == CHECKER_BLACK && 
		board[currPoint.x + moveDirect.dx][currPoint.y + moveDirect.dy].figure != CHECKER_BLACK &&
		InRange(currPoint.x, MIN_POS, MAX_POS - 2) && InRange(currPoint.y, MIN_POS, MAX_POS - 2))
	{
		CaptureVec interResult;
		CaptureVec result;
		Capture capture = { startPoint, currPoint };
		captureVec.push_back(capture);
		board[currPoint.x][currPoint.y].figure = BLANK;
		while (InRange(currPoint.x, MIN_POS - 1, MAX_POS - 1) && InRange(currPoint.y, MIN_POS - 1, MAX_POS - 1) &&
			board[currPoint.x][currPoint.y].figure != CHECKER_BLACK)
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
		if (board[currPoint.x + moveDirect.dx][currPoint.y + moveDirect.dy].figure == CHECKER_BLACK &&
			board[currPoint.x][currPoint.y].figure == CHECKER_BLACK)
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
	CaptureVec resultVec = MakeCapture(NO_MOVE, startPoint, board, captureVec);
	for (size_t i = 0; i < resultVec.size(); i++)
	{
		std::cout << "whitePos " << resultVec.at(i).whitePos.x << " " << resultVec.at(i).whitePos.y << std::endl;
	}
	return 0;
}