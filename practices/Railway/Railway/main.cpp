#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

struct Segment
{
	int begin;
	int end;
};

struct SegmentEnd
{
	int end;
	bool accounted;
};

struct Predicat
{
	bool operator()(const Segment& firstSegment, const Segment& secondSegment)
	{
		if (firstSegment.begin < secondSegment.begin)
		{
			return true;
		}
		else
		{
			if (firstSegment.begin > secondSegment.begin)
			{
				return false;
			}
			else
			{
				return (firstSegment.end < secondSegment.end);
			}
		}
	}
};


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Wrong argument number\n";

		return 1;
	}
	std::ifstream inputFile(argv[1]);
	int N;
	int M;
	std::vector<Segment> segmentVec;
	std::vector<SegmentEnd> segmentEndVec;
	inputFile >> N;
	inputFile >> M;
	for (int i = 0; i < N; i++)
	{
		Segment currSegment;
		inputFile >> currSegment.begin;
		inputFile >> currSegment.end;
		segmentVec.push_back(currSegment);
	}
	std::cout << N << " " << M << " " << std::endl;
	std::sort(segmentVec.begin(), segmentVec.end(), Predicat());

	int count = 0;
	int result = 0;
	int currPoint;
	bool inSegment = false;
	for (int i = 0; i < segmentVec.size() - 1; i++)
	{
		currPoint = segmentVec[i].begin;
		count++;
		if (count >= M && !inSegment)
		{
			inSegment = true;
			result++;
		}
		if (segmentVec[i].end < segmentVec[i + 1].begin)
		{
			inSegment = false;
			count--;
		}
	}
	std::cout << result << std::endl;

	return 0;
}