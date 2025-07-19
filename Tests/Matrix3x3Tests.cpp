#include "gtest/gtest.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/matrix_transform_2d.hpp>
#include "System/Matrix3x3.hpp"
#include "System/Mathf.hpp"
include <iostream>
#include <cmath>

void PrintMatrix(const System::Matrix3x3& m) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << m.m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void PrintMatrix(const glm::mat3& m) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }
}




// Helper function to compare System::Matrix3x3 and glm::mat3
:testing::AssertionResult CompareMatrices(const System::Matrix3x3& sysMat, const glm::mat3& glmMat) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (std::abs(sysMat.m[j][i] - glmMat[j][i]) > 1e-6) {
                return ::testing::AssertionFailure() << "Matrices differ at (" << i << ", " << j << ")\n"
                    << "System Matrix:\n"
                    << sysMat.m[0][0] << " " << sysMat.m[0][1] << " " << sysMat.m[0][2] << "\n"
                    << sysMat.m[1][0] << " " << sysMat.m[1][1] << " " << sysMat.m[1][2] << "\n"
                    << sysMat.m[2][0] << " " << sysMat.m[2][1] << " " << sysMat.m[2][2] << "\n"
                    << "GLM Matrix:\n"
                    << glmMat[0][0] << " " << glmMat[0][1] << " " << glmMat[0][2] << "\n"
                    << glmMat[1][0] << " " << glmMat[1][1] << " " << glmMat[1][2] << "\n"
                    << glmMat[2][0] << " " << glmMat[2][1] << " " << glmMat[2][2] << "\n";
            }
        }
    }
    return ::testing::AssertionSuccess();
}
TEST(Matrix3x3Test, Identity) {
    System::Matrix3x3 sysIdentity = System::Matrix3x3::identity();
    glm::mat3 glmIdentity = glm::mat3(1.0f);
    EXPECT_TRUE(CompareMatrices(sysIdentity, glmIdentity));
}

TEST(Matrix3x3Test, Translation) {
    System::Matrix3x3 sysTranslate = System::Matrix3x3::Translate(10.0f, 20.0f);
    glm::mat3 glmTranslate = glm::translate(glm::mat3(1.0f), glm::vec2(10.0f, 20.0f));
    EXPECT_TRUE(CompareMatrices(sysTranslate, glmTranslate));
}

TEST(Matrix3x3Test, Scale) {
    System::Matrix3x3 sysScale = System::Matrix3x3::Scale(2.0f, 3.0f);
    glm::mat3 glmScale = glm::scale(glm::mat3(1.0f), glm::vec2(2.0f, 3.0f));
    EXPECT_TRUE(CompareMatrices(sysScale, glmScale));
}

TEST(Matrix3x3Test, Rotation) {
    float angle = 45.0f;
    System::Matrix3x3 sysRotate = System::Matrix3x3::Rotate(angle);
    glm::mat3 glmRotate = glm::rotate(glm::mat3(1.0f), angle * System::Mathf::Deg2Rad);
    EXPECT_TRUE(CompareMatrices(sysRotate, glmRotate));
}
TEST(Matrix3x3Test, TRS) {
    System::Vector2 translation(10.0f, 20.0f);
    float rotation = 30.0f;
    System::Vector2 scale(2.0f, 3.0f);
    System::Matrix3x3 sysTRS = System::Matrix3x3::TRS(translation, rotation, scale);

    glm::mat3 model = glm::mat3(1.0f);
    model = glm::translate(model, glm::vec2(translation.x, translation.y));
    model = glm::rotate(model, rotation * System::Mathf::Deg2Rad);
    model = glm::scale(model, glm::vec2(scale.x, scale.y));
    glm::mat3 glmTRS = model;

    EXPECT_TRUE(CompareMatrices(sysTRS, glmTRS));
}

TEST(Matrix3x3Test, TRSOptimised) {
    System::Vector2 translation(10.0f, 20.0f);
    float rotation = 30.0f;
    System::Vector2 scale(2.0f, 3.0f);
    System::Matrix3x3 sysTRSOptimised = System::Matrix3x3::TRSOptimised(translation, rotation, scale);

    glm::mat3 model = glm::mat3(1.0f);
    model = glm::translate(model, glm::vec2(translation.x, translation.y));
    model = glm::rotate(model, rotation * System::Mathf::Deg2Rad);
    model = glm::scale(model, glm::vec2(scale.x, scale.y));
    glm::mat3 glmTRS = model;

    EXPECT_TRUE(CompareMatrices(sysTRSOptimised, glmTRS));
}

TEST(Matrix3x3Test, Multiplication) {
    System::Matrix3x3 sysMat1 = System::Matrix3x3::Translate(10, 20);
    System::Matrix3x3 sysMat2 = System::Matrix3x3::Scale(2, 3);
    System::Matrix3x3 sysResult = sysMat1 * sysMat2;

    glm::mat3 glmMat1 = glm::translate(glm::mat3(1.0f), glm::vec2(10.0f, 20.0f));
    glm::mat3 glmMat2 = glm::scale(glm::mat3(1.0f), glm::vec2(2.0f, 3.0f));
    glm::mat3 glmResult = glmMat1 * glmMat2;

    EXPECT_TRUE(CompareMatrices(sysResult, glmResult));
}

TEST(Matrix3x3Test, Invert) {
    System::Matrix3x3 sysMat = System::Matrix3x3::TRS({10, 20}, 45, {2, 3});
    System::Matrix3x3 sysInverse = sysMat.Invert();

    glm::mat3 model = glm::mat3(1.0f);
    model = glm::translate(model, glm::vec2(10.0f, 20.0f));
    model = glm::rotate(model, 45.0f * System::Mathf::Deg2Rad);
    model = glm::scale(model, glm::vec2(2.0f, 3.0f));
    glm::mat3 glmInverse = glm::inverse(model);

    EXPECT_TRUE(CompareMatrices(sysInverse, glmInverse));
}