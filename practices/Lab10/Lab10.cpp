// Lab10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <map>

void UpdateResult(std::string& result, std::string& currResult);

int main(int argc, char* argv[])
{
	std::string inputFileName = argv[1];
	std::ifstream inputFile(inputFileName);

	size_t N;
	inputFile >> N;

	// в first лежит сам символ, в second - его позиция
	std::map<char, int> currAlphabet;
	std::string inputString;
	std::string result;
	std::string currResult;
	inputFile >> inputString;
	for (size_t pos = 0; pos < N; ++pos)
	{
		char currCh = inputString[pos];
		if (currAlphabet.count(currCh))
		{
			pos = currAlphabet[currCh];
			UpdateResult(result, currResult);
			currResult.clear();
			currAlphabet.clear();
		}
		else
		{
			currAlphabet[currCh] = pos;
			currResult += currCh;
		}
		
		if (pos == (N - 1))
		{
			UpdateResult(result, currResult);
		}
	}

	std::cout << result << std::endl;

	return 0;
}


void UpdateResult(std::string& result, std::string& possibleResult)
{
	if (result.empty())
	{
		result = possibleResult;

		return;
	}
	
	if (result.size() < possibleResult.size())
	{
		result = possibleResult;

		return;
	}
	
	if (possibleResult.size() == result.size() && possibleResult < result)
	{
		result = possibleResult;
	}

	return;
}