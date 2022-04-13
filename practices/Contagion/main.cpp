// Lab7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

struct Point
{
    int x;
    int y;
};

int main(int argc, char * argv[])
{
    std::string inputFileName = argv[1];
    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open())
    {
        std::cout << "Can't open input file " << inputFileName;

        return 1;
    }
    int N;
    int M;
    inputFile >> N;
    inputFile >> M;

    std::deque<Point> MPoints;

    std::vector<std::vector<int>> field;
    std::vector<int> tempVector;
    for (int x = 0; x < N; ++x)
    {
        tempVector = {};
        for (int y = 0; y < N; ++y)
        {
            tempVector.push_back(0);
        }
        field.push_back(tempVector);
    }
    size_t counter = 1;

    for (int i = 0; i < M; i++)
    {
        Point currPoint;
        inputFile >> currPoint.x;
        inputFile >> currPoint.y;
        currPoint.x -= 1;
        currPoint.y -= 1;
        field[currPoint.x][currPoint.y] = counter;
        MPoints.push_back(currPoint);
    }

    
    while (!MPoints.empty())
    {
        size_t currMPointsAmount = MPoints.size();

        for (size_t i = 0; i < currMPointsAmount; ++i)
        {
            Point currPoint = MPoints.front();
            MPoints.pop_front();
            if (currPoint.x != 0 && field[currPoint.x - 1][currPoint.y] == 0)
            {
                field[currPoint.x - 1][currPoint.y] = counter;
                MPoints.push_back({ currPoint.x - 1, currPoint.y });
            }
            if (currPoint.x != N - 1 && field[currPoint.x + 1][currPoint.y] == 0)
            {
                field[currPoint.x + 1][currPoint.y] = counter;
                MPoints.push_back({ currPoint.x + 1, currPoint.y });
            }
            if (currPoint.y != 0 && field[currPoint.x][currPoint.y - 1] == 0)
            {
                field[currPoint.x][currPoint.y - 1] = counter;
                MPoints.push_back({ currPoint.x, currPoint.y - 1 });
            }
            if (currPoint.y != N - 1 && field[currPoint.x][currPoint.y + 1] == 0)
            {
                field[currPoint.x][currPoint.y + 1] = counter;
                MPoints.push_back({ currPoint.x, currPoint.y + 1 });
            }
        }

        ++counter;
    }

    std::cout << counter - 2 << std::endl;

    return 0;
}