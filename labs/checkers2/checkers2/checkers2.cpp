#include <iostream>
#include <stack>
#include <optional>
#include <string>
#include <fstream>
#include <array>

constexpr size_t MAX_POS = 8;
const std::string outputFileName = "OUTPUT.TXT";

typedef std::array<std::array<char, MAX_POS>, MAX_POS> Board;

void FillBoard(std::ifstream& inputFile, Board& board)
{
	for (size_t row = 0; row < MAX_POS; ++row)
	{
		std::string line;
		std::getline(inputFile, line);

		for (size_t col = 0; col < MAX_POS; ++col)
		{
			board[row][col] = line[col];
		}
	}

	return;
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

int main(int argc, char* argv[])
{
	Board board;
	std::ifstream inputFile(argv[1]);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}
	FillBoard(inputFile, board);

	return 0;
}