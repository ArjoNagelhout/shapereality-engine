//
// Created by Arjo Nagelhout on 02/12/2023.
//

#include "math/vector.h"
#include "math/vector.inl"

#include "math/quaternion.h"
#include "math/quaternion.inl"

#include "math/matrix.h"
#include "math/matrix.inl"

#include "gtest/gtest.h"

using namespace math;

TEST(Matrix, TranslationRotationScale)
{
	vec3 translation{{1, 2, 3}};
	Quaternion rotation = Quaternion::identity;
	vec3 scale{{1, 1, 1}};

	mat4 trs = createTranslationRotationScaleMatrix(translation, rotation, scale);

	mat4 expectedTRS = mat4{{{
								{{1, 2, 3, 4}},
								{{1, 2, 3, 4}},
								{{1, 2, 3, 4}},
								{{1, 2, 3, 4}}
	}}};

	//EXPECT_EQ(trs, expectedTRS);
}

TEST(Matrix, PerspectiveProjection)
{

}

TEST(Matrix, OrthographicProjection)
{

}