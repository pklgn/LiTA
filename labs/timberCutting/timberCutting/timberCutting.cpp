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

typedef std::vector<size_t> CuttingPoints;
typedef std::vector<int> CostRow;
typedef std::vector<CostRow> CostTable;

bool ValidateFile(const std::ifstream& inputFile);
CostTable GetCostTable(CuttingPoints& cuttingPoints);

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
	size_t cuttingPoint;
	CuttingPoints cuttingPoints;
	cuttingPoints.push_back(MIN_CUTTING_POINT);
	for (size_t i = 0; i < N; ++i)
	{
		inputFile >> cuttingPoint;
		cuttingPoints.push_back(cuttingPoint);
	}
	cuttingPoints.push_back(L);
	CostTable costTable = GetCostTable(cuttingPoints);

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

CostTable GetCostTable(CuttingPoints& cuttingPoints)
{
	int row = INITIAL_ROW;
	int column = row + 1;
	int tableSize = cuttingPoints.size();
	CostTable result(tableSize, CostRow(tableSize, INITIAL_COST));
	while (column < cuttingPoints.size())
	{
		int currColumn = column;
		int currRow = INITIAL_ROW;
		for (; currRow < cuttingPoints.size() && currColumn < cuttingPoints.size(); ++currRow, ++currColumn)
		{
			if (currColumn <= row + 1)
			{
				continue;
			}

			int tempCost = NO_COST;
			for (int i = currRow + 1; i < currColumn; ++i)
			{
				
				int currCost = result[currRow][i] + result[i][currColumn] + cuttingPoints[currColumn] - cuttingPoints[currRow];
				if (currCost < tempCost || tempCost == NO_COST)
				{
					tempCost = currCost;
				}
			}
			if (tempCost != NO_COST)
			{
				result[currRow][currColumn] = tempCost;
			}
		}
		column++;
	}

	return result;
}