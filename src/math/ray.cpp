//
// Created by Arjo Nagelhout on 30/11/2023.
//

#include "ray.h"
#include "vector.inl"



namespace math
{
	std::string Ray::toString() const
	{
		std::stringstream result{};
		result << "{origin: " << _origin << ", direction: " << _direction << "}";
		return result.str();
	}
}