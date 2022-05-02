// stringSearchKMP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

typedef std::vector<int> Prefix;
typedef std::map<int, std::vector<int>> SearchOccurrences;
typedef std::vector<std::string> Templates;

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";

struct TemplateSearchInfo
{
	std::string substring;
	Prefix prefix;
	size_t index = 0;
};

struct TextSearchInfo
{
	std::vector<TemplateSearchInfo> templates;
	std::string text;
};

bool ValidateFile(const std::ifstream& inputFile);
void ReadSearchInfo(std::ifstream& inputFile, TextSearchInfo& searchInfo);
void Search(TextSearchInfo& searchInfo, SearchOccurrences& results);
bool SearchInfoCompare(TemplateSearchInfo& first, TemplateSearchInfo& second);
void PrintResults(std::ofstream& outputFile, TextSearchInfo& searchInfo, SearchOccurrences& results);

int main()
{
	std::ifstream inputFile(INPUT_FILE_NAME);
	bool status = ValidateFile(inputFile);
	if (!status)
	{
		return 1;
	}

	TextSearchInfo searchInfo;
	ReadSearchInfo(inputFile, searchInfo);

	std::sort(searchInfo.templates.begin(), searchInfo.templates.end(), SearchInfoCompare);

	SearchOccurrences results;
	Search(searchInfo, results);

	std::ofstream outputFile(OUTPUT_FILE_NAME);
	PrintResults(outputFile, searchInfo, results);

	return 0;
}

void SetPrefix(TemplateSearchInfo& templateInfo)
{
	templateInfo.prefix[0] = 0;
	int i = 1;
	int j = 0;
	while (i < templateInfo.substring.size())
	{
		if (templateInfo.substring[i] != templateInfo.substring[j])
		{
			if (j == 0)
			{
				templateInfo.prefix[i] = 0;
				++i;
			}
			else
			{
				j = templateInfo.prefix[j - 1];
			}
		}
		else
		{
			templateInfo.prefix[i] = j + 1;
			++i;
			++j;
		}
	}
}

void SearchTemplate(size_t searchPostion, std::string& text, TemplateSearchInfo& templateInfo,
	SearchOccurrences& results)
{
	size_t k = searchPostion;
	size_t l = 0;

	if (text.size() < k + templateInfo.substring.size())
	{
		return;
	}

	while (k < text.size())
	{
		if (text[k] == templateInfo.substring[l])
		{
			++k;
			++l;
			if (l == templateInfo.substring.size())
			{
				results[k - l].push_back(templateInfo.index);
				size_t delta = templateInfo.substring.size() - templateInfo.prefix[templateInfo.prefix.size() - 1];
				SearchTemplate(k - l + delta, text, templateInfo, results);
				break;
			}
			if (k == text.size())
			{
				results[k].push_back(templateInfo.index);
			}
		}
		else if (l == 0)
		{
			++k;
			if (k == text.size())
			{
				results[k].push_back(templateInfo.index);
			}
		}
		else
		{
			l = templateInfo.prefix[l - 1];
		}
	}
}

void ReadSubstrings(std::ifstream& inputFile, std::vector<TemplateSearchInfo>& templates)
{
	size_t N;
	inputFile >> N;

	for (size_t i = 0; i < N; ++i)
	{
		std::string substring;
		inputFile >> substring;
		Prefix prefix(substring.size(), -1);
		TemplateSearchInfo templateInfo({ substring, prefix });
		SetPrefix(templateInfo);
		templates.push_back(templateInfo);
	}

	return;
}

void ReadSearchInfo(std::ifstream& inputFile, TextSearchInfo& searchInfo)
{
	ReadSubstrings(inputFile, searchInfo.templates);
	inputFile >> searchInfo.text;

	return;
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

void Search(TextSearchInfo& searchInfo, SearchOccurrences& results)
{
	for (size_t i = 0; i < searchInfo.templates.size(); ++i)
	{
		searchInfo.templates[i].index = i;
		SearchTemplate(0, searchInfo.text, searchInfo.templates[i], results);
	}

	return;
}

bool SearchInfoCompare(TemplateSearchInfo& first, TemplateSearchInfo& second)
{
	return first.substring < second.substring;
}

void PrintResults(std::ofstream& outputFile, TextSearchInfo& searchInfo, SearchOccurrences& results)
{
	for (auto& result : results)
	{
		for (auto& substringPos : result.second)
		{
			outputFile << searchInfo.templates[substringPos].substring << " ";
			if (result.first == searchInfo.text.size())
			{
				outputFile << "No";
			}
			else
			{
				outputFile << result.first + 1;
			}
			outputFile << std::endl;
		}
	}

	return;
}