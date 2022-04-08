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
#include <limits>
#include <cmath>

constexpr size_t MIN_TIMBER_CUTTING_POINT = 0;
constexpr size_t NO_COST = 0;
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
	cuttingPoints.push_back(MIN_TIMBER_CUTTING_POINT);
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
	int row = 0;
	int column = row + 1;
	int tableSize = cuttingPoints.size();
	CostTable result(tableSize, CostRow(tableSize, -1));
	while (column < cuttingPoints.size())
	{
		int currColumn = column;
		int currRow = 0;
		for (; currRow < cuttingPoints.size() && currColumn < cuttingPoints.size(); ++currRow, ++currColumn)
		{
			if (currColumn <= row)
			{
				continue;
			}

			if (column - row == 1)
			{
				result[currRow][currColumn] = 0;
			}
			else
			{
				result[currRow][currColumn] = std::abs(static_cast<int>(cuttingPoints[column] - cuttingPoints[currRow]));
			}
			//std::cout << "row " << row << " column " << currColumn << std::endl;
		}
		column++;
	}

	return result;
}