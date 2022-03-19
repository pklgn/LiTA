#include <iostream>
#include <vector>
#include <fstream>

const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

int main(int argc, char* argv[])
{
	std::ifstream inputFile(argv[1]);
	size_t N;
	inputFile >> N;
	std::vector<size_t> initialSet;
	std::vector<size_t> inversionVec;
	std::vector<size_t> resultVec;
	for (size_t i = 0; i < N; ++i)
	{
		size_t currInv;
		inputFile >> currInv;
		inversionVec.push_back(currInv);
		initialSet.push_back(i + 1);
		resultVec.push_back(i);
	}
	for (int i = N - 1; i >= 0; --i)
	{
		size_t currInv = inversionVec[i];
		resultVec[i] = initialSet[i - currInv];
		initialSet.erase(initialSet.begin() + initialSet.size() - 1 - currInv);
	}

	std::ofstream outputFile(OUTPUT_FILE_NAME);
	for (int i = 0; i < N; i++)
	{
		outputFile << resultVec[i] << ' ';
	}

	for (int i = 0; i < N; i++)
	{
		std::cout << resultVec[i] << ' ';
	}
	std::cout << std::endl;

	return 0;
}