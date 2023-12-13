//
// Created by Arjo Nagelhout on 13/12/2023.
//

#include "gtest/gtest.h"

#include <iostream>

TEST(ScratchPad, Fun)
{
	int count = 1000;

	label:
	count--;
	std::cout << count << std::endl;

	if (count <= 0)
	{
		goto end;
	}

	goto label;

	end:
	std::cout << "we're done here" << std::endl;
}

TEST(ScratchPad, NestedLoops)
{
	for (int i = 0; i < 10; i++)
	{
		bool one = false;

		for (int z = 0; z < 10; z++)
		{
			for (int r = 0; r < 10; r++)
			{
				if (r == 5)
				{
					// go to 1
					one = true;
					break;
				}
			}

			if (one)
			{
				break;
			}
		}

		// 1

	}
}