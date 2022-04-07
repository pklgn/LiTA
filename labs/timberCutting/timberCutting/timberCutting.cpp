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

constexpr size_t MIN_TIMBER_CUTTING_POINT = 0;
constexpr size_t NO_COST = 0;
const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

typedef std::vector<size_t> CuttingPoints;
typedef std::vector<std::vector<size_t>> CostTable;

bool ValidateFile(const std::ifstream& inputFile);

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
	CostTable result;
	for (size_t i = 1, j = 0; i < cuttingPoints.size(); ++j)
	{
		
	}
}