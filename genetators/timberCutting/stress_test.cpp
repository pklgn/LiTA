#include <iostream>

constexpr int MAX_POINT_POSITION = 500;

int main(int argc, char** argv[])
{
	std::cout << MAX_POINT_POSITION << ' '
		<< MAX_POINT_POSITION - 1 << std::endl;
	for (size_t i = 1; i < MAX_POINT_POSITION - 1; ++i)
	{
		std::cout << i << ' ';
	}
	
	std::cout << std::endl;
}