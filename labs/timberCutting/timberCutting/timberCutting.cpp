#include <iostream>
#include <fstream>
#include <vector>

constexpr size_t MIN_TIMBER_CUTTING_POINT = 0;
const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

typedef std::vector<size_t> CuttingPoints;

bool ValidateFile(const std::ifstream& inputFile);
size_t GetMinTimberLengthCut(std::vector<size_t>& cuttingPoints);

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
	std::cout << GetMinTimberLengthCut(cuttingPoints) << std::endl;

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

size_t CalculateTimberLength(size_t leftBound, size_t rightBound)
{
	return rightBound - leftBound;
}

size_t GetMinTimberLengthCut(std::vector<size_t>& cuttingPoints)
{
	size_t result = cuttingPoints.back();
	size_t minCuttingPoint = 0;
	for (size_t point = 1; point < cuttingPoints.size() - 1; ++point)
	{
		size_t timberLength = CalculateTimberLength(cuttingPoints.at(point - 1), cuttingPoints.at(point + 1));
		if (timberLength < result)
		{
			result = timberLength;
			minCuttingPoint = point;
		}
	}
	cuttingPoints.erase(cuttingPoints.begin() + minCuttingPoint);
	while (cuttingPoints.size() > 2)
	{
		result += GetMinTimberLengthCut(cuttingPoints);
	}

	return result;
}