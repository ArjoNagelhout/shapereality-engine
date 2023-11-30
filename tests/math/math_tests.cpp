//
// Created by Arjo Nagelhout on 30/11/2023.
//

#include <iostream>

//#include "../../src/math/vector.h"

#include "gtest/gtest.h"

int main(int argc, char* argv[])
{
	std::cout << "hello tests" << std::endl;
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}