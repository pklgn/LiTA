/*
* 3.3. ������ ����� (7)
* �� �������� �������� ���� ������ L ������. �������� �����, � ������� ���������� ������� �������. 
* ��������� ������ ������� ����� ����� �������������� �����. ��������� ������� �������� �������� � 
* ��������� �������� �����. ��������, ���� ������ 10 ������, ������� ����� ��������� �� ���������� 
* 2, 4 � 7 ������ ����� ������ ������� ���������. ����� ��������� ������� �� ������� 2, ����� 4 � 
* ����� 7. ��������� ����� 10+8+6=24. � ���� ������� ��������� �� ������� 4, ����� 2 � ����� 7, �� 
* ��������� �������� 10+4+6=20. ��������� �������� ���������, ������� ���������� �� ����������� ��������� 
* �������.
* 
* �������� C++ 17
* ����� ����������: Visual Studio 2019, MSVC
* ��: Windows 10
*
* ��������: ������� �����, ��-21
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
