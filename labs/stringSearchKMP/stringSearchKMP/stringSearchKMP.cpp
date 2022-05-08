/*
* 13.4. Поиск по нескольким образцам методом КМП (4)
* Требуется найти все вхождения любого из образцов в текст. Результаты поиска не должны зависеть от 
* регистра букв, то есть каждая буква в образце и тексте может быть как строчной, так и прописной.
* 
* 
* Стандарт C++ 17
* Среда разработки: Visual Studio 2019, MSVC
* ОС: Windows 10
*
* Выполнил: Ермаков Павел, ПС-21
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

typedef std::vector<int> Prefix;
typedef std::map<int, std::vector<int>> SearchOccurrences;
typedef std::vector<std::string> Templates;

const std::string INPUT_FILE_NAME = "INPUT.TXT";
const std::string OUTPUT_FILE_NAME = "OUTPUT.TXT";
constexpr int NO_POSITION = INT_MAX;

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
	setlocale(0, "");
	TextSearchInfo searchInfo;
	ReadSearchInfo(inputFile, searchInfo);
	std::transform(searchInfo.text.begin(), searchInfo.text.end(), searchInfo.text.begin(), 
		[](unsigned char c) { return std::tolower(c); });

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
	size_t i = 1;
	int j = 0;
	std::string lowerSubstring;
	std::transform(templateInfo.substring.begin(), templateInfo.substring.end(),
		std::back_inserter(lowerSubstring), [](unsigned char c) { return std::tolower(c); });
	while (i < templateInfo.substring.size())
	{
		if (lowerSubstring[i] != lowerSubstring[j])
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

size_t SearchTemplate(size_t searchPostion, std::string& text, std::string& substring,
	TemplateSearchInfo& templateInfo, SearchOccurrences& results)
{
	size_t k = searchPostion;
	size_t l = 0;

	if (text.size() < k + templateInfo.substring.size() && searchPostion != 0)
	{
		return NO_POSITION;
	}
	else if (text.size() < k + templateInfo.substring.size())
	{
		results[text.size()].push_back(templateInfo.index);

		return NO_POSITION;
	}

	while (k < text.size())
	{
		if (text[k] == substring[l])
		{
			++k;
			++l;
			if (l == templateInfo.substring.size())
			{
				results[k - l].push_back(templateInfo.index);
				size_t delta = templateInfo.substring.size() - templateInfo.prefix[templateInfo.prefix.size() - 1];

				return k - l + delta;
			}
			if (k == text.size() && searchPostion == 0)
			{
				results[k].push_back(templateInfo.index);
			}
		}
		else if (l == 0)
		{
			++k;
			if (k == text.size() && searchPostion == 0)
			{
				results[k].push_back(templateInfo.index);
			}
		}
		else
		{
			l = templateInfo.prefix[l - 1];
		}
	}

	return NO_POSITION;
}

void ReadSubstrings(std::ifstream& inputFile, std::vector<TemplateSearchInfo>& templates)
{
	std::string line;
	size_t N;
	std::getline(inputFile, line);
	N = std::stoi(line);

	for (size_t i = 0; i < N; ++i)
	{
		std::string substring;
		std::getline(inputFile, substring);
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
	std::getline(inputFile, searchInfo.text);

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
		std::string substring;
		std::transform(searchInfo.templates[i].substring.begin(), searchInfo.templates[i].substring.end(),
			std::back_inserter(substring), [](unsigned char c) { return std::tolower(c); });
		size_t startPosition = 0;
		do
			startPosition = SearchTemplate(startPosition, searchInfo.text, substring, searchInfo.templates[i], results);
		while (startPosition != NO_POSITION);
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