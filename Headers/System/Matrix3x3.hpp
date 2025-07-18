#ifndef _SYSTEM_MATRIX3X3_H
#define _SYSTEM_MATRIX3X3_H

#include <cmath>
#include "Vector2.hpp"
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
				struct {
					/// <summary>row 1 column 1.</summary>
					float M11;
					/// <summary>row 1 column 2.</summary>
					float M12;
					/// <summary>row 1 column 3.</summary>
					float M13;
					/// <summary>row 2 column 1.</summary>
					float M21;
					/// <summary>row 2 column 2.</summary>
					float M22;
					/// <summary>row 2 column 3.</summary>
					float M23;
					/// <summary>row 3 column 1.</summary>
					float M31;
					/// <summary>row 3 column 2.</summary>
					float M32;
					/// <summary>row 3 column 3.</summary>
					float M33;
				};
			};
			Matrix3x3();
			Matrix3x3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33);

			static Matrix3x3 Translate(float tx, float ty);
			static Matrix3x3 Scale(float sx, float sy);
			static Matrix3x3 Rotate(float radians);
			static Matrix3x3 identity();
			Matrix3x3 transpose() const;
			Matrix3x3 Invert() const;
			float Determinant() const;
			Matrix3x3 Adjugate() const;

	



		

		Matrix3x3 operator*(const Matrix3x3& B) const {
			const Matrix3x3& A = *this;
			Matrix3x3 R;
			// First column
			R.m[0] = A.m[0]*B.m[0] + A.m[3]*B.m[1] + A.m[6]*B.m[2];
			R.m[1] = A.m[1]*B.m[0] + A.m[4]*B.m[1] + A.m[7]*B.m[2];
			R.m[2] = A.m[2]*B.m[0] + A.m[5]*B.m[1] + A.m[8]*B.m[2];
			// Second column
			R.m[3] = A.m[0]*B.m[3] + A.m[3]*B.m[4] + A.m[6]*B.m[5];
			R.m[4] = A.m[1]*B.m[3] + A.m[4]*B.m[4] + A.m[7]*B.m[5];
			R.m[5] = A.m[2]*B.m[3] + A.m[5]*B.m[4] + A.m[8]*B.m[5];
			// Third column
			R.m[6] = A.m[0]*B.m[6] + A.m[3]*B.m[7] + A.m[6]*B.m[8];
			R.m[7] = A.m[1]*B.m[6] + A.m[4]*B.m[7] + A.m[7]*B.m[8];
			R.m[8] = A.m[2]*B.m[6] + A.m[5]*B.m[7] + A.m[8]*B.m[8];
			return R;
	}
	 


	};
}
#endif