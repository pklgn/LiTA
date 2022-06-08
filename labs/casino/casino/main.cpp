#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string INPUT_FILE_NAME = "OUTPUT.TXT";

bool ValidateFile(const std::ifstream& inputFile);

typedef std::vector<int> Accounts;

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}

	Accounts accounts;
	// read file
	size_t N;
	inputFile >> N;
	for (size_t i = 0; i < N; ++i)
	{
		int account;
		inputFile >> account;
		accounts.push_back(account);
	}

	// accounting


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