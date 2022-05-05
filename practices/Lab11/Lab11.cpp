// Lab11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

int Rsq(const int a, std::vector<int>& fenvickTree);
int Rsq2(const int i, const int j, std::vector<int>& fenvickTree);
void Add(const int k, const int D, std::vector<int>& fenvickTree);
void Update(int k, const int V, std::vector<int>& fenvickTree);
void PrintFenvickTree(std::ostream& outputStream, std::vector<int>& fenvickTree);

int main()
{
	size_t N;
	std::cin >> N;
	std::vector<int> initTree;
	for (size_t i = 0; i < N; ++i)
	{
		int number;
		std::cin >> number;
		initTree.push_back(number);
	}
	
	std::vector<int> fenvickTree;

	for (size_t k = 0; k < initTree.size(); ++k)
	{
		int i = k & (k + 1);
		int kResult = 0;
		for (; i < k + 1; ++i)
		{
			kResult += initTree[i];
		}
		fenvickTree.push_back(kResult);
	}
	
	std::string command;
	while (true)
	{
		std::cin >> command;
		if (command == "Rsq")
		{
			int firstArg;
			int secondArg;
			std::cin >> firstArg >> secondArg;
			std::cout << "Result is " << Rsq2(firstArg, secondArg, fenvickTree) << std::endl;
		}
		else if (command == "Add")
		{
			int firstArg;
			int secondArg;
			std::cin >> firstArg >> secondArg;
			Add(firstArg, secondArg, fenvickTree);
			PrintFenvickTree(std::cout, fenvickTree);
		}
		else if (command == "Update")
		{
			int firstArg;
			int secondArg;
			std::cin >> firstArg >> secondArg;
			Update(firstArg, secondArg, fenvickTree);
			PrintFenvickTree(std::cout, fenvickTree);
		}
		else if (command == "Print")
		{
			PrintFenvickTree(std::cout, fenvickTree);
		}
		else if (command == "Exit")
		{
			break;
		}
		else
		{
			std::cout << "Unknown command\n";
		}
	}

	return 0;
}

int Rsq(int a, std::vector<int>& fenvickTree)
{
	int result = 0;
	while (a >= 0)
	{
		result += fenvickTree[a];
		a = (a & (a + 1)) - 1;
	}
	return result;
}

int Rsq2(int i, int j, std::vector<int>& fenvickTree)
{
	return Rsq(j, fenvickTree) - Rsq(i - 1, fenvickTree);
}

void Add(int k, int D, std::vector<int>& fenvickTree)
{
	while (k < fenvickTree.size())
	{
		fenvickTree[k] += D;
		k = k | (k + 1);
	}
}

void Update(int k, const int V, std::vector<int>& fenvickTree)
{
	int Ak = Rsq2(k, k, fenvickTree);
	Add(k, V - Ak, fenvickTree);
}

void PrintFenvickTree(std::ostream& outputStream, std::vector<int>& fenvickTree)
{
	for (auto& number : fenvickTree)
	{
		outputStream << number << " ";
	}

	outputStream << std::endl;
}