/*
* 3.3. Распил бруса (7)
* На пилораму привезли брус длиной L метров. Известны места, в которых необходимо сделать распилы. 
* Стоимость одного распила равна длине распиливаемого бруса. Различный порядок распилов приводит к 
* различным итоговым ценам. Например, брус длиной 10 метров, который нужно распилить на расстоянии 
* 2, 4 и 7 метров можно пилить разными способами. Можно распилить сначала на отметке 2, потом 4 и 
* потом 7. Стоимость будет 10+8+6=24. А если сначала распилить на отметке 4, потом 2 и затем 7, то 
* стоимость составит 10+4+6=20. Требуется написать программу, которая определяла бы минимальную стоимость 
* распила.
* 
* Стандарт C++ 17
* Среда разработки: Visual Studio 2019, MSVC
* ОС: Windows 10
*
* Выполнил: Ермаков Павел, ПС-21
*/

#include <iostream>
#include <fstream>
#include <vector>

constexpr size_t INITIAL_ROW = 0;
constexpr size_t INITIAL_COST = 0;
constexpr size_t MIN_CUTTING_POINT = 0;
constexpr size_t NO_COST = -1;
const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

typedef std::vector<int> CuttingPoints;
typedef std::vector<int> CostRow;
typedef std::vector<CostRow> CostTable;

struct TablePosition
{
	size_t x;
	size_t y;
};

bool ValidateFile(const std::ifstream& inputFile);
CostTable GetMinCostTable(CuttingPoints& cuttingPoints);
void ReadCuttingPoints(std::ifstream& inputFile, CuttingPoints& cuttingPoints, size_t N, size_t L);

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	if (!ValidateFile(inputFile))
	{
		return 1;
	}

	size_t L;
	size_t N;

	inputFile >> L >> N;
	CuttingPoints cuttingPoints;
	ReadCuttingPoints(inputFile, cuttingPoints, N, L);
	CostTable costTable = GetMinCostTable(cuttingPoints);


	std::ofstream outputFile(OUTPUT_FILE_NAME);
	outputFile << costTable[INITIAL_ROW][costTable.size() - 1] << std::endl;

	#ifdef _DEBUG
	std::cout << costTable[INITIAL_ROW][costTable.size() - 1] << std::endl;
	#endif

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

void ReadCuttingPoints(std::ifstream& inputFile, CuttingPoints& cuttingPoints, size_t N, size_t L)
{
	size_t cuttingPoint;
	cuttingPoints.push_back(MIN_CUTTING_POINT);
	for (size_t i = 0; i < N; ++i)
	{
		inputFile >> cuttingPoint;
		cuttingPoints.push_back(cuttingPoint);
	}
	cuttingPoints.push_back(L);
}

int GetMinCostTableCell(TablePosition& position, CostTable& costTable, CuttingPoints& cuttingPoints)
{
	int cost = NO_COST;
	int length = cuttingPoints[position.y] - cuttingPoints[position.x];
	for (size_t i = position.x + 1; i < position.y; ++i)
	{
		int currCost = costTable[position.x][i] + costTable[i][position.y] + length;
		if (currCost < cost || cost == NO_COST)
		{
			cost = currCost;
		}
	}

	return cost;
}

void FillMinCostTable(CostTable& costTable, CuttingPoints& cuttingPoints)
{
	size_t column = INITIAL_ROW + 1;
	while (column < cuttingPoints.size())
	{
		TablePosition currPosition = { INITIAL_ROW, column };
		for (;currPosition.x < cuttingPoints.size() && currPosition.y < cuttingPoints.size(); 
			++currPosition.x, ++currPosition.y)
		{
			if (currPosition.y <= currPosition.x + 1)
			{
				continue;
			}

			int currCost = GetMinCostTableCell(currPosition, costTable, cuttingPoints);
			if (currCost != NO_COST)
			{
				costTable[currPosition.x][currPosition.y] = currCost;
			}
		}
		column++;
	}
}

CostTable GetMinCostTable(CuttingPoints& cuttingPoints)
{
	int tableSize = cuttingPoints.size();
	CostTable result(tableSize, CostRow(tableSize, INITIAL_COST));
	FillMinCostTable(result, cuttingPoints);

	return result;
}
