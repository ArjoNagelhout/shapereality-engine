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

TEST(Matrix, Multiplication)
{
	mat4 matrix1 = mat4{{{
							 {{1, 2, 3, 4}},
							 {{5, 6, 7, 8}},
							 {{9, 10, 11, 12}},
							 {{13, 14, 15, 16}},
	}}};
	mat4 matrix2 = mat4{{{
							 {{17, 18, 19, 20}},
							 {{21, 22, 23, 24}},
							 {{25, 26, 27, 28}},
							 {{29, 30, 31, 32}},
						 }}};

	mat4 expected = mat4{{{
							  {{250, 260, 270, 280}},
							  {{618, 644, 670, 696}},
							  {{986, 1028, 1070, 1112}},
							  {{1354, 1412, 1470, 1528}},
	}}};

	mat4 multiplied = matrix1 * matrix2;

	ASSERT_EQ(multiplied, expected);
}

TEST(Matrix, VectorConstructor)
{
	mat4 expected = mat4{{{
							  {{1, 2, 3, 4}},
							  {{5, 10, 11, 2}},
							  {{6, 9, 5, 23}},
							  {{7, 8, 10, 10}},
	}}};

	vec4 row1 = vec4{{1, 2, 3, 4}};
	vec4 row2 = vec4{{5, 10, 11, 2}};
	vec4 row3 = vec4{{6, 9, 5, 23}};
	vec4 row4 = vec4{{7, 8, 10, 10}};

	mat4 test = mat4{{row1, row2, row3, row4}};

	ASSERT_EQ(test, expected);
}

TEST(Matrix, Inverse)
{
	mat2 matrix2{{{
							{{1, 2}},
							{{3, 4}}
	}}};
	mat2 test2{{{
						  {{5, 6}},
						  {{7, 8}}
	}}};
	mat2 multiplied2 = matrix2 * test2;
	mat2 inverse2 = matrix2.inverse();
	mat2 inverted2 = inverse2 * multiplied2;
	ASSERT_EQ(test2, inverted2);

	mat3 matrix3{{{
							{{1, 2, 3}},
							{{4, 5, 6}},
							{{7, 8.5, 9}}
	}}};
	mat3 test3{{{
						   {{10, 11, 12}},
						   {{13, 14, 15}},
						   {{16, 17, 18}}
	}}};
	mat3 multiplied3 = matrix3 * test3;
	mat3 inverse3 = matrix3.inverse();
	mat3 inverted3 = inverse3 * multiplied3;
	ASSERT_EQ(test3, inverted3);
	//std::cout << "difference: " << test3 - inverted3 << std::endl;

	mat4 matrix4{{{
					  {{10, 11, 12, 20}},
					  {{13, 14, 15, 11}},
					  {{16, 17, 18, 10}},
					  {{6, 1.5, 2, 50}}
	}}};

	mat4 test4{{{
					{{1, 6, 23,  6}},
					{{45, 32, 34, 10}},
					{{6, 32, 57, 1}},
					{{10, 21, 865, 54}}
	}}};

	mat4 multiplied4 = matrix4 * test4;
	mat4 inverse4 = matrix4.inverse();
	mat4 inverted4 = inverse4 * multiplied4;
	ASSERT_EQ(test4, inverted4);
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

//	EXPECT_EQ(projection, expected);
}

TEST(Matrix, OrthographicProjection)
{

}

TEST(Matrix, Equality)
{

}