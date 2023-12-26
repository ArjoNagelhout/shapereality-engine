#include "gtest/gtest.h"

#include <iostream>

void printHoiNeneng(int getal)
{
	std::cout << "Hoi Neneng " << getal << "\n";
}

TEST(Neneng, Voorbeeld)
{
	for (int i = 1; i < 100000000; i++)
	{
		printHoiNeneng(i);
	}
}