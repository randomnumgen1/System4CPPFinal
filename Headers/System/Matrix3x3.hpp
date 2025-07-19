#ifndef _SYSTEM_MATRIX3X3_H
#define _SYSTEM_MATRIX3X3_H

#include <cmath>
#include "Vector2.hpp"
#include <ostream>
//Matrix3x3 should be column major
//functions and constructors should be checked for correctness
namespace System {
	struct Matrix3x3 {
		private:
		
		public:
			//column major

			// memory layout:
			//
			//             row no (=vertical)
			//               |  0   1   2 
			//            ---+------------
			//            0  | m11 m12 m13
			// column no  1  | m21 m22 m23
			// (=horiz)   2  | m31 m32 m33
			union {
				float raw[9];
				float m[3][3];
				struct {
					/// <summary>row 0 column 0.</summary>
					float M00;
					/// <summary>row 1 column 0.</summary>
					float M10;
					/// <summary>row 2 column 0.</summary>
					float M20;
					/// <summary>row 0 column 1.</summary>
					float M01;
					/// <summary>row 1 column 1.</summary>
					float M11;
					/// <summary>row 2 column 1.</summary>
					float M21;
					/// <summary>row 0 column 2.</summary>
					float M02;
					/// <summary>row 1 column 2.</summary>
					float M12;
					/// <summary>row 2 column 2.</summary>
					float M22;
				};
			};
			Matrix3x3();
			Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22);

			static Matrix3x3 Translate(float tx, float ty);
			static Matrix3x3 Scale(float sx, float sy);
			static Matrix3x3 Rotate(float radians);
			static Matrix3x3 TRS(Vector2 translation, float rotation, Vector2 scale);
			static Matrix3x3 identity();
			Matrix3x3 transpose() const;
			Matrix3x3 Invert() const;
			float Determinant() const;
			Matrix3x3 Adjugate() const;
			static bool CompareMajorityIndependant(Matrix3x3 matrixA, Matrix3x3 matrixB);
	



		

			Matrix3x3 operator*(const Matrix3x3& other) const {
				Matrix3x3 result;

				result.raw[0] = this->raw[0] * other.raw[0] + this->raw[3] * other.raw[1] + this->raw[6] * other.raw[2];
				result.raw[1] = this->raw[1] * other.raw[0] + this->raw[4] * other.raw[1] + this->raw[7] * other.raw[2];
				result.raw[2] = this->raw[2] * other.raw[0] + this->raw[5] * other.raw[1] + this->raw[8] * other.raw[2];

				result.raw[3] = this->raw[0] * other.raw[3] + this->raw[3] * other.raw[4] + this->raw[6] * other.raw[5];
				result.raw[4] = this->raw[1] * other.raw[3] + this->raw[4] * other.raw[4] + this->raw[7] * other.raw[5];
				result.raw[5] = this->raw[2] * other.raw[3] + this->raw[5] * other.raw[4] + this->raw[8] * other.raw[5];

				result.raw[6] = this->raw[0] * other.raw[6] + this->raw[3] * other.raw[7] + this->raw[6] * other.raw[8];
				result.raw[7] = this->raw[1] * other.raw[6] + this->raw[4] * other.raw[7] + this->raw[7] * other.raw[8];
				result.raw[8] = this->raw[2] * other.raw[6] + this->raw[5] * other.raw[7] + this->raw[8] * other.raw[8];
				return result;
			}

	 


	};

	std::ostream& operator<<(std::ostream& os, const Matrix3x3& m) {
		os << std::fixed << std::setprecision(3);
		os << "[[" << m.M00 << ", " << m.M10 << ", " << m.M20 << "]," << std::endl
			<< " [" << m.M01 << ", " << m.M11 << ", " << m.M21 << "]," << std::endl
			<< " [" << m.M02 << ", " << m.M12 << ", " << m.M22 << "]]" << std::endl;
		return os;
	}

}
#endif