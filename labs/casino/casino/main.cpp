#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

typedef std::vector<int> Accounts;
typedef std::vector<size_t> OperationSequence;

bool ValidateFile(const std::ifstream& inputFile);
void CalculateMaxExpenses(Accounts& accounts, OperationSequence& operationSequence);
void PrintMaxExpenses(Accounts& accounts, OperationSequence& operationSequence);

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
	OperationSequence resultSequence;
	CalculateMaxExpenses(accounts, resultSequence);

	PrintMaxExpenses(accounts, resultSequence);

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

void Subtract(size_t minuendIndex, Accounts& accounts, OperationSequence& operationSequence)
{
	accounts[minuendIndex] -= accounts[minuendIndex + 1];
	accounts.erase(accounts.begin() + minuendIndex + 1);
	operationSequence.push_back(minuendIndex);

	return;
}

void CalculateMaxExpenses(Accounts& accounts, OperationSequence& operationSequence)
{
	size_t currIndex = 1;
	while (currIndex < accounts.size())
	{
		while (currIndex < accounts.size() && accounts[currIndex] < 0)
		{
			++currIndex;
		}
		while (currIndex < accounts.size() && accounts[currIndex] >= 0)
		{
			if (currIndex - 1 != 0)
			{
				Subtract(currIndex - 1, accounts, operationSequence);
			}
			else
			{
				++currIndex;
			}
		}
	}
	while (accounts.size() > 1)
	{
		Subtract(0, accounts, operationSequence);
	}
}

void PrintMaxExpenses(Accounts& accounts, OperationSequence& operationSequence)
{
	std::ofstream outputFile(OUTPUT_FILE_NAME);

	outputFile << accounts[0] << std::endl;
	for (auto& index : operationSequence)
	{
		outputFile << index + 1 << ' ';
	}

	outputFile << std::endl;
}
