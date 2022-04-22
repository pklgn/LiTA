// Lab9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

typedef std::vector<std::string> Words;
typedef std::pair<int, int> CharSpec;
typedef std::map<char, CharSpec> CharMap;

bool Compare(std::string& firstString, std::string& secondString);
void PrintResult(int length, Words words, std::ostream& outputStream);

int main(int argc, char* argv[])
{
	std::string inputFileName = argv[1];
	std::ifstream inputFile(inputFileName);

	if (!inputFile.is_open())
	{
		std::cout << "Can't open file\n";

		return 1;
	}

	std::string keyWord;
	inputFile >> keyWord;
	Words words;

	while (!inputFile.eof())
	{
		std::string currWord;
		inputFile >> currWord;
		words.push_back(currWord);
	}

	CharMap charMap;
	
	for (char ch: keyWord)
	{
		if (charMap.count(ch))
		{
			charMap[ch].first += 1;
		}
		else
		{
			charMap[ch].first = 1;
		}
	}

	int resultLength = 0;
	Words resultWords;
	
	for (auto& currWord: words)
	{
		bool isCorrect = true;
		for (char currWordCh: currWord)
		{
			if (charMap.count(currWordCh))
			{
				charMap[currWordCh].second += 1;
				if (charMap[currWordCh].first < charMap[currWordCh].second)
				{
					isCorrect = false;

					break;
				}
			}
			else
			{
				isCorrect = false;

				break;
			}
		}
		if (isCorrect)
		{
			resultLength += currWord.size();
			resultWords.push_back(currWord);
		}
		for (auto& chSpec : charMap)
		{
			chSpec.second.second = 0;
		}
	}
	std::sort(resultWords.begin(), resultWords.end(), Compare);
	
	PrintResult(resultLength, resultWords, std::cout);

	return 0;
}

bool Compare(std::string& firstString, std::string& secondString)
{
	if (firstString.size() < secondString.size())
	{
		return false;
	}
	else if (firstString.size() > secondString.size())
	{
		return true;
	}
	else
	{
		return firstString < secondString;
	}
}

void PrintResult(int length, Words words, std::ostream& outputStream)
{
	outputStream << length << std::endl;
	for (auto& resultWord : words)
	{
		outputStream << resultWord << std::endl;
	}
}