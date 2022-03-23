// Lab6.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <vector>

struct Vect
{
    int x;
    int y;
};

struct Point
{
    int x;
    int y;
};

int GetVectMult(Vect lVect, Vect rVect)
{
    return ((lVect.x * rVect.y) - (rVect.x * lVect.y));
}

int main(int argc, char* argv[])
{
    std::ifstream inputFile;
    inputFile.open(argv[1]);
    if (!inputFile)
    {
        std::cout << "File isn't open\n";
        return 1;
    }
    size_t N;
    inputFile >> N;

    std::vector<Point> pointVec;
    Point point;
    for (int i = 0; i < N; i++)
    {
        inputFile >> point.x;
        inputFile >> point.y;
        pointVec.push_back(point);
    }
    std::vector<Vect> vectVec;
    Vect vect;
    for (size_t i = 1; i < pointVec.size(); ++i)
    {
        Point pointB = pointVec[i];
        Point pointA = pointVec[i - 1];
        vect.x = pointB.x - pointA.x;
        vect.y = pointB.y - pointA.y;
        vectVec.push_back(vect);
    }
    size_t count = 0;
    for (size_t i = 1; i < vectVec.size(); ++i)
    {
        Vect vectB;
        Vect vectA;
        vectB = vectVec[i];
        vectA = vectVec[i - 1];
        if (vectB.x * vectA.y - vectA.x * vectB.y < 0)
        {
            count++;
        }
    }

    std::cout << count << std::endl;

    return 0;
}

