//
// Created by Arjo Nagelhout on 02/12/2023.
//

#include "math/utility.h"

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
    Vector3 translation{{1, 2, 3}};
    Quaternion rotation = Quaternionf::identity;
    Vector3 scale{{1, 1, 1}};

    Matrix4 trs = createTRSMatrix(translation, rotation, scale);

    Matrix4 expectedTRS = Matrix4{
        1, 2, 3, 4,
        1, 2, 3, 4,
        1, 2, 3, 4,
        1, 2, 3, 4
    };

    //EXPECT_EQ(trs, expectedTRS);
}

TEST(Matrix, Multiplication)
{
    Matrix4 matrix1 = Matrix4{
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16,
    };
    Matrix4 matrix2 = Matrix4{
        17, 18, 19, 20,
        21, 22, 23, 24,
        25, 26, 27, 28,
        29, 30, 31, 32,
    };

    Matrix4 expected = Matrix4{
        250, 260, 270, 280,
        618, 644, 670, 696,
        986, 1028, 1070, 1112,
        1354, 1412, 1470, 1528,
    };

    Matrix4 multiplied = matrix1 * matrix2;

    ASSERT_EQ(multiplied, expected);
}

TEST(Matrix, InverseTranslation)
{
    Matrix4 matrix{
        1, 0, 0, -2.0f,
        0, 1, 0, 1.5f,
        0, 0, 1, -10.0f,
        0, 0, 0, 1,
    };

    Matrix4 inverse = matrix.getInverse();
}

TEST(Matrix, Inverse)
{
    Matrix2 matrix2{
        1, 2,
        3, 4
    };
    Matrix2 test2{
        5, 6,
        7, 8
    };
    Matrix2 multiplied2 = matrix2 * test2;
    Matrix2 inverse2 = matrix2.getInverse();
    Matrix2 inverted2 = inverse2 * multiplied2;
    ASSERT_EQ(test2, inverted2);

    Matrix3 matrix3{
        1, 2, 3,
        4, 5, 6,
        7, 8.5, 9
    };
    Matrix3 test3{
        10, 11, 12,
        13, 14, 15,
        16, 17, 18,
    };
    Matrix3 multiplied3 = matrix3 * test3;
    Matrix3 inverse3 = matrix3.getInverse();
    Matrix3 inverted3 = inverse3 * multiplied3;
    ASSERT_EQ(test3, inverted3);

    Matrix4 matrix4{
        10, 11, 12.5, 120,
        13, -14, 15, 11,
        16, 17, 18, 10,
        6, 1.5, 2, 50,
    };

    Matrix4 test4{
        10, 6, 23, 30,
        45, 32, 82, 10,
        6, 100, 57.5, 0.5,
        10, 21, 20.523, 54,
    };

    Matrix4 multiplied4 = matrix4 * test4;
    Matrix4 inverse4 = matrix4.getInverse();
    Matrix4 inverted4 = inverse4 * multiplied4;
    ASSERT_TRUE(test4.approximatelyEquals(inverted4));
}

TEST(Matrix, PerspectiveProjection)
{
    float const fov = degreesToRadians(60.f);
    float const aspectRatio = 1.f;
    float const zNear = 0.f;
    float const zFar = 500.f;

    std::cout << "fov: " << fov << std::endl;

    Matrix4 projection = createPerspectiveProjectionMatrix(fov, aspectRatio, zNear, zFar);
    Matrix4 expected{
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
    };

//    EXPECT_EQ(projection, expected);
}

TEST(Matrix, OrthographicProjection)
{

}

TEST(Matrix, Equality)
{

}