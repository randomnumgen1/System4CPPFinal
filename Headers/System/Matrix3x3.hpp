#ifndef _SYSTEM_MATRIX3X3_H
#define _SYSTEM_MATRIX3X3_H

#include <cmath>
#include "Vector2.hpp"
//struct should be column major
namespace System {
	struct Matrix3x3 {
		private:
		
		public:
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
		Matrix3x3 Invert() const {
			float det = m[0] * (m[4] * m[8] - m[7] * m[5]) -
				m[3] * (m[1] * m[8] - m[7] * m[2]) +
				m[6] * (m[1] * m[5] - m[4] * m[2]);

			if (det == 0.f) {
				return Matrix3x3::identity();
			}

			float invDet = 1.f / det;

			Matrix3x3 inv;
			inv.m[0] = (m[4] * m[8] - m[7] * m[5]) * invDet;
			inv.m[3] = (m[6] * m[5] - m[3] * m[8]) * invDet;
			inv.m[6] = (m[3] * m[7] - m[6] * m[4]) * invDet;
			inv.m[1] = (m[7] * m[2] - m[1] * m[8]) * invDet;
			inv.m[4] = (m[0] * m[8] - m[6] * m[2]) * invDet;
			inv.m[7] = (m[6] * m[1] - m[0] * m[7]) * invDet;
			inv.m[2] = (m[1] * m[5] - m[4] * m[2]) * invDet;
			inv.m[5] = (m[3] * m[2] - m[0] * m[5]) * invDet;
			inv.m[8] = (m[0] * m[4] - m[3] * m[1]) * invDet;

			return inv;
		}




		Matrix3x3(){
			m[0] = 1.f; m[3] = 0.f; m[6] = 0.f;
			m[1] = 0.f; m[4] = 1.f; m[7] = 0.f;
			m[2] = 0.f; m[5] = 0.f; m[8] = 1.f;			
		}
		Matrix3x3(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8) {
			m[0] = m0; m[1] = m1; m[2] = m2;
			m[3] = m3; m[4] = m4; m[5] = m5;
			m[6] = m6; m[7] = m7; m[8] = m8;
		}
		static Matrix3x3 Translate(float tx, float ty) {
			return Matrix3x3(
				1.f, 0.f, 0.f,
				0.f, 1.f, 0.f,
				tx , ty , 1.f
			);
		}
		static Matrix3x3 Scale(float sx, float sy) {
			return Matrix3x3(
				sx , 0.f, 0.f,
				0.f, sy , 0.f,
				0.f, 0.f, 1.f
			);
		}
		static Matrix3x3 Rotate(float radians) {
			float c = std::cos(radians);
			float s = std::sin(radians);
			return Matrix3x3(
				 c ,  s , 0.f,
				-s ,  c , 0.f,
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