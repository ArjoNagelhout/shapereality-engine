//
// Created by Arjo Nagelhout on 02/12/2023.
//

#include "math/utils.h"

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
	float const fov = degreesToRadians(60.f);
	float const aspectRatio = 1.f;
	float const zNear = 0.f;
	float const zFar = 500.f;

	std::cout << "fov: " << fov << std::endl;

	mat4 projection = createPerspectiveProjectionMatrix(fov, aspectRatio, zNear, zFar);
	mat4 expected{{{
					   {{0, 0, 0, 0}},
					   {{0, 0, 0, 0}},
					   {{0, 0, 0, 0}},
					   {{0, 0, 0, 0}},
	}}};

	EXPECT_EQ(projection, expected);
}

TEST(Matrix, OrthographicProjection)
{

}

TEST(Matrix, Equality)
{

}