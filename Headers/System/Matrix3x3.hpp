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
			//            0  | m00 m10 m20
			// column no  1  | m01 m11 m21
			// (=horiz)   2  | m02 m12 m22
			union {
				float m[9];
				struct {
					float m00, m10, m20;
					float m01, m11, m21;
					float m02, m12, m22;
				};
			};
			Matrix3x3();
			Matrix3x3(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22);

			static Matrix3x3 Translate(float tx, float ty);
			static Matrix3x3 Scale(float sx, float sy);
			static Matrix3x3 Rotate(float radians);
			static Matrix3x3 identity();
			Matrix3x3 transpose() const;
			Matrix3x3 Invert() const;



		Matrix3x3 Invert1() const {
			float det = m[0] * (m[4] * m[8] - m[7] * m[5]) -
				m[3] * (m[1] * m[8] - m[7] * m[2]) +
				m[6] * (m[1] * m[5] - m[4] * m[2]);

			if (det == 0.f) {
				return Matrix3x3::identity();
			}

			float invDet = 1.f / det;

			Matrix3x3 inv;
			//First column
			inv.m[0] = (m[4] * m[8] - m[7] * m[5]) * invDet;
			inv.m[3] = (m[2] * m[7] - m[1] * m[8]) * invDet;
			inv.m[6] = (m[1] * m[5] - m[2] * m[4]) * invDet;
			// Second column
			inv.m[1] = (m[5] * m[6] - m[3] * m[8]) * invDet;
			inv.m[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
			inv.m[7] = (m[2] * m[3] - m[0] * m[5]) * invDet;
			// Third column
			inv.m[2] = (m[3] * m[7] - m[5] * m[6]) * invDet;
			inv.m[5] = (m[1] * m[6] - m[0] * m[7]) * invDet;
			inv.m[8] = (m[0] * m[4] - m[3] * m[1]) * invDet;

			return inv;
		}
		Matrix3x3 Invert2() const {
			float det =
				m[0] * (m[4] * m[8] - m[5] * m[7]) -
				m[3] * (m[1] * m[8] - m[2] * m[7]) +
				m[6] * (m[1] * m[5] - m[2] * m[4]);

			if (det == 0.f) {
				return Matrix3x3::identity();
			}

			float invDet = 1.f / det;

			Matrix3x3 inv;

			// First column
			inv.m[0] = (m[4] * m[8] - m[5] * m[7]) * invDet;
			inv.m[1] = -(m[1] * m[8] - m[2] * m[7]) * invDet;
			inv.m[2] = (m[1] * m[5] - m[2] * m[4]) * invDet;

			// Second column
			inv.m[3] = -(m[3] * m[8] - m[5] * m[6]) * invDet;
			inv.m[4] = (m[0] * m[8] - m[2] * m[6]) * invDet;
			inv.m[5] = -(m[0] * m[5] - m[2] * m[3]) * invDet;

			// Third column
			inv.m[6] = (m[3] * m[7] - m[4] * m[6]) * invDet;
			inv.m[7] = -(m[0] * m[7] - m[1] * m[6]) * invDet;
			inv.m[8] = (m[0] * m[4] - m[1] * m[3]) * invDet;

			return inv;
		}




		

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