/*
* 14.4. Казино (9)
* Казино в день своего юбилея решило сделать подарок клиентам. Составляется список клиентов в порядке их прихода.
* Каждый клиент имеет общий баланс игры, выраженный положительным или отрицательным целым числом. Казино устраивает
* общий фуршет, затраты на который устанавливаются следующим образом. По списку клиентов формируется соответствующий
* список балансов. Значения двух соседних элементов списка заменяются разностью предыдущего и последующего элементов
* до тех пор, пока не останется единственное число, определяющее величину затрат на фуршет. Найти такую последовательность
* операций, чтобы оставшееся число было максимальным. Определить этот максимум.
* 
* 
* Стандарт C++ 14
* Среда разработки: Visual Studio 2019, MSVC
* ОС: Windows 10
*
* Выполнил: Ермаков Павел, ПС-21
*/


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
	size_t N;
	inputFile >> N;
	for (size_t i = 0; i < N; ++i)
	{
		int account;
		inputFile >> account;
		accounts.push_back(account);
	}

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
