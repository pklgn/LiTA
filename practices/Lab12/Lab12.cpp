// Lab12.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <iostream>

void Build(int i, int L, int R, std::vector<int>& A, std::vector<int>& T);
int Rmq(int L, int R, std::vector<int>& T);

int main()
{
	std::vector<int> A = { 2, 7, 9, 4, 6, 5, 8 };
	std::vector<int> T(4*(A.size()), 0);

	Build(0, 0, A.size(), A, T);
	std::cout << Rmq(1, 4, T) << std::endl;
	std::cout << Rmq(3, 6, T) << std::endl;


	return 0;
}

void Build(int i, int L, int R, std::vector<int>& A, std::vector<int>& T)
{
	int m;
	if (L == R)
	{
		T[i] = A[L];
	}
	else
	{
		m = (L + R) / 2;
		Build(2 * i + 1, L, m, A, T);
		Build(2 * i + 2, m + 1, R, A, T);
		T[i] = T[2 * i + 1] + T[2 * i + 2];
	}
}

int Rmq(int L, int R, std::vector<int>& T)
{
	int result = INT_MIN;
	int N = (T.size()) / 2;
	L += N - 1;
	R += N - 1;

	while (L <= R)
	{
		if (L & 1)
		{
			result = std::max(result, T[L]);
		}
		if (!(R & 1))
		{
			result = std::max(result, T[R]);
		}
		L = (L + 1) / 2;
		R = (R - 1) / 2;
	}

	return result;
}