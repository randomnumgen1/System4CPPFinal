#include "System/Vector3.hpp"
#include "System/Matrix4x4.hpp"


/*
Matrix4x4 Tests
*/

//Constructors
//this test is the correct values and float index DO NOT MODIFY
TEST(BasicMatrix4x4Test, PerspectiveTest1) {
	Matrix4x4 mat = Matrix4x4::Perspective(60f, 16 / 9f, 1f, 1000f);
	
	EXPECT_FLOAT_EQ(mat.raw[0], 0.9742786);
	EXPECT_FLOAT_EQ(mat.raw[1], 	0.00000f);
	EXPECT_FLOAT_EQ(mat.raw[2], 0.00000f);
	EXPECT_FLOAT_EQ(mat.raw[3], 0.00000f);
	
	EXPECT_FLOAT_EQ(mat.raw[4], 0.00000f);
	EXPECT_FLOAT_EQ(mat.raw[5], 1.732051);
	EXPECT_FLOAT_EQ(mat.raw[6], 0.00000f);
	EXPECT_FLOAT_EQ(mat.raw[7], 0.00000f);
	
	EXPECT_FLOAT_EQ(mat.raw[8], 0.00000f);
	EXPECT_FLOAT_EQ(mat.raw[9], 0.00000f);
	EXPECT_FLOAT_EQ(mat.raw[10],  -1.002002);
	EXPECT_FLOAT_EQ(mat.raw[11], -1);
	
	EXPECT_FLOAT_EQ(mat.raw[12], 0.00000f);
	EXPECT_FLOAT_EQ(mat.raw[13], 0.00000f);
	EXPECT_FLOAT_EQ(mat.raw[14], -2.002002);
	EXPECT_FLOAT_EQ(mat.raw[15], 0.00000f);
	
}
    


 










//EXPECT_NEAR(,,1e-5f);


/*
----------------------------
Vector3 Tests
----------------------------
*/


//Constructors
TEST(BasicVector3Test, Constructordefault) {
    System::Vector3 sysVec;
    EXPECT_EQ(sysVec.x, 0.0f);
    EXPECT_EQ(sysVec.y, 0.0f);
    EXPECT_EQ(sysVec.z, 0.0f);
}
TEST(BasicVector3Test, ConstructorAll) {
    System::Vector3 sysVec(1.0f, 2.0f, 3.0f);
    EXPECT_EQ(sysVec.x, 1.0f);
    EXPECT_EQ(sysVec.y, 2.0f);
    EXPECT_EQ(sysVec.z, 3.0f);
}
//Addition
TEST(BasicVector3Test, Addition) {
    System::Vector3 add1(1, 2, 3);
    System::Vector3 add2(3, 2, 1);
	System::Vector3 result;
	
    result = add1 + add2;
    EXPECT_EQ(result.x, 4.0f);
    EXPECT_EQ(result.y, 4.0f);
    EXPECT_EQ(result.z, 4.0f);
}
//Subtraction
TEST(BasicVector3Test, Subtraction) {
    System::Vector3 add1(5, 4, 3);
    System::Vector3 add2(3, 2, 1);
	System::Vector3 result;
	
    result = add1 - add2;
    EXPECT_EQ(result.x, 4.0f);
    EXPECT_EQ(result.y, 4.0f);
    EXPECT_EQ(result.z, 4.0f);
}





