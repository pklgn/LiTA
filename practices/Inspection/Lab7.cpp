// Lab7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
typedef std::pair<int, std::vector<int>> DoctorDependencies;

int main(int argc, char** argv)
{
    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cout << "Can't open file\n";

        return 1;
    }

    int K; //number of doctors
    inputFile >> K;

    std::vector<DoctorDependencies> dependenciesVec;
    for (int i = 0; i < K; ++i)
    {
        int dependenciesAmount;
        inputFile >> dependenciesAmount;

        std::vector<int> doctorDependencies;
        for (int j = 0; j < dependenciesAmount; ++j)
        {
            int currDoctorDependency;
            inputFile >> currDoctorDependency;
            doctorDependencies.push_back(currDoctorDependency);
        }

        DoctorDependencies currValue = std::make_pair(dependenciesAmount, doctorDependencies);
        dependenciesVec.push_back(currValue);
    }

    int count = 0;
    std::vector<int> resultIndexes;
    bool completed = false;
    while (count < K)
    {
        int prevCount = count;
        for (int i = 0; i < static_cast<int>(dependenciesVec.size()); ++i)
        {
            if (dependenciesVec[i].first == 0)
            {
                dependenciesVec[i].first -= 1;
                resultIndexes.push_back(i + 1);
                ++count;

                for (DoctorDependencies& dependency : dependenciesVec)
                {
                    auto it = std::find(dependency.second.begin(), dependency.second.end(), i + 1);
                    if (it != dependency.second.end())
                    {
                        dependency.second.erase(it);
                        dependency.first -= 1;
                    }
                }
            }
        }
        if (prevCount == count)
        {
            completed = false;
            std::cout << "NO\n";

            break;
        }
    }
    if (count == K)
    {
        completed = true;
    }

    if (completed)
    {
        std::cout << "YES\n";
        for (const int index : resultIndexes)
        {
            std::cout << index << '\n';
        }

        std::cout << std::endl;
    }

    return 0;
}
