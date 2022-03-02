#include <iostream>
#include <cmath>
#include <fstream>

int getMinAmount(int N)
{
    int minAmount = 5;
    if (N == 0)
    {
        return 0;
    }
    for (int i = sqrt(N); i >= 1; i--)
    {
        int currAmount = getMinAmount(N - i * i) + 1;
        if (currAmount == 1)
        {
            return currAmount;
        }
        if (currAmount < minAmount)
        {
            minAmount = currAmount;
        }
    }

    return minAmount;
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
    int result = getMinAmount(N);
    std::cout << result << std::endl;
}