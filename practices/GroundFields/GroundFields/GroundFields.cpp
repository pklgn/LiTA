#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

int getMinAmount(int N, std::vector<int>& minAmountVec, std::vector<int>& values)
{
    int minAmount = 5;
    if (N == 0)
    {
        return 0;
    }
    for (int i = 1; i <= sqrt(N); i++)
    {
        int currAmount = minAmountVec[N - i * i] + 1;
        if (currAmount == 1)
        {
            values.push_back(i * i);

            return currAmount;
        }
        if (currAmount < minAmount)
        {
            minAmount = currAmount;
        }
    }

    return minAmount;
}
void fillMinAmountVec(std::vector<int>& minAmountVec, int N, std::vector<int>& last)
{
    for (int i = 1; i <= N; i++)
    {
        int minAmount = 5;
        minAmountVec.push_back(5);
        last.push_back(1);
        for (int j = 1; j <= sqrt(i); j++)
        {
            int currAmount = minAmountVec[i - j * j] + 1;
            if (currAmount == 1)
            {
                minAmountVec[i] = currAmount;
                last[i] = j * j;
                break;
            }
            if (currAmount < minAmount)
            {
                last[i] = j * j;
                minAmountVec[i] = currAmount;
            }
        }
        //std::cout << i << " = " << minAmountVec[i] << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Please enter the file name" << std::endl;

        return 1;
    }
    std::ifstream sourceFile(argv[1]);
    int N;
    sourceFile >> N;
    std::vector<int> minAmountVec;
    std::vector<int> last;
    std::vector<int> values;
    minAmountVec.push_back(0);
    last.push_back(0);
    fillMinAmountVec(minAmountVec, N, last);
    int result = getMinAmount(N, minAmountVec, values);
    std::cout << result << std::endl;
}