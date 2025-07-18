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
		static Matrix3x3 identity() {
			return Matrix3x3{
				1.f, 0.f, 0.f,
				0.f, 1.f, 0.f,
				0.f, 0.f, 1.f
			};
		}
		Matrix3x3 transpose() const{
			Matrix3x3 T;
			T.m[0] = m[0];  T.m[1] = m[3];  T.m[2] = m[6];
			T.m[3] = m[1];  T.m[4] = m[4];  T.m[5] = m[7];
			T.m[6] = m[2];  T.m[7] = m[5];  T.m[8] = m[8];
			return T;
		}
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



		Matrix3x3(){
			m[0] = 1.f; m[3] = 0.f; m[6] = 0.f;
			m[1] = 0.f; m[4] = 1.f; m[7] = 0.f;
			m[2] = 0.f; m[5] = 0.f; m[8] = 1.f;			
		}
		Matrix3x3(float m00, float m10, float m20, float m01, float m11, float m21, float m02, float m12, float m22) {
			m[0] = m00; m[1] = m10; m[2] = m20;
			m[3] = m01; m[4] = m11; m[5] = m21;
			m[6] = m02; m[7] = m12; m[8] = m22;
		}
		static Matrix3x3 Translate(float tx, float ty) {
			return Matrix3x3(
				1.f, 0.f, tx,
				0.f, 1.f, ty,
				0.f, 0.f, 1.f
			);
		}
		static Matrix3x3 Scale(float sx, float sy) {
			return Matrix3x3(
				sx, 0.f, 0.f,
				0.f, sy, 0.f,
				0.f, 0.f, 1.f
			);
		}
		static Matrix3x3 Rotate(float radians) {
			float c = std::cos(radians);
			float s = std::sin(radians);
			return Matrix3x3(
				c, -s, 0.f,
				s, c, 0.f,
				0.f, 0.f, 1.f
			);
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
		Vector2 operator*(const Vector2& v) const {
			float x = m[0] * v.x + m[3] * v.y + m[6];
			float y = m[1] * v.x + m[4] * v.y + m[7];
			return Vector2(x, y);
		}


	};
}
#endif