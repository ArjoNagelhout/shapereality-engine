//
// Created by Arjo Nagelhout on 20/12/2023.
//

#include "gtest/gtest.h"

#include "input/types.h"
#include <iostream>

using namespace input;

TEST(Input, Types)
{
	auto modifier = static_cast<KeyboardModifier_>(
		KeyboardModifier_Meta |
		KeyboardModifier_Shift |
		KeyboardModifier_Option |
		KeyboardModifier_Control |
		KeyboardModifier_CapsLock);
	std::string string = toString(modifier);
	std::cout << string << std::endl;
}