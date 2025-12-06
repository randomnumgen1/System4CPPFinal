/*
Matrix4x4 Tests
*/

//Constructors
TEST(BasicMatrix4x4Test, Constructordefault) {
	Matrix4x4 mat = Matrix4x4::Perspective(60f, 16 / 9f, 1f, 1000f);
	
	EXPECT_EQ(mat[0], 0.97428f);
	EXPECT_EQ(mat[1], 	0.00000f);
	EXPECT_EQ(mat[2], 0.00000f);
	EXPECT_EQ(mat[3], 0.00000f);
	EXPECT_EQ(mat[4], 0.00000f);
	EXPECT_EQ(mat[5], 1.73205f);
	EXPECT_EQ(mat[6], 0.00000f);
	EXPECT_EQ(mat[7], 0.00000f);
	EXPECT_EQ(mat[8], 0.00000f);
	EXPECT_EQ(mat[9], 0.00000f);
	EXPECT_EQ(mat[10], -1.00200f);
	EXPECT_EQ(mat[11], -2.00200f);
	EXPECT_EQ(mat[12], 0.00000f);
	EXPECT_EQ(mat[13], 0.00000f);
	EXPECT_EQ(mat[14], -1.00000f);
	EXPECT_EQ(mat[15], 0.00000f);
	
}



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





