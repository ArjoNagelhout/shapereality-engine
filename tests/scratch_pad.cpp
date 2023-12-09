//
// Created by Arjo Nagelhout on 09/12/2023.
//

#include "gtest/gtest.h"

#include <memory>

// performance testing for casting

struct Base
{
	[[nodiscard]] virtual bool isBase() const
	{
		return true;
	}
};

struct Inherited final : public Base
{
	[[nodiscard]] bool isBase() const override
	{
		return false;
	}
};

bool staticIsBase(std::shared_ptr<Base> const& b)
{
	auto const& staticCast = static_cast<Inherited const&>(*b);
	return staticCast.isBase();
}

bool dynamicIsBase(std::shared_ptr<Base> const& b)
{
	auto const& dynamicCast = dynamic_cast<Inherited const&>(*b);
	return dynamicCast.isBase();
}

TEST(ScratchPad, Casting)
{
	std::shared_ptr<Base> inherited = std::make_unique<Inherited>();
	std::shared_ptr<Base> base = std::make_unique<Base>();

	std::cout << "is base (dynamic): " << dynamicIsBase(inherited) << std::endl;
	std::cout << "is base (static): " << staticIsBase(inherited) << std::endl;

	// staticIsBase does not throw an error, dynamicIsBase does.
	// dangerous :)
	//std::cout << "is base (dynamic): " << dynamicIsBase(base) << std::endl;
	std::cout << "is base (static): " << staticIsBase(base) << std::endl;
}