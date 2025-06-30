#ifndef _SYSTEM_MATRIX3X3_H
#define _SYSTEM_MATRIX3X3_H

namespace System {
	struct Matrix3x3 {
		private:
		
		public:
		float m[9];
		
		static const Matrix3x3 identity;
		Matrix3x3 transpose() const{
			Matrix3x3 T;
			T.m[0] = m[0];  T.m[1] = m[3];  T.m[2] = m[6];
			T.m[3] = m[1];  T.m[4] = m[4];  T.m[5] = m[7];
			T.m[6] = m[2];  T.m[7] = m[5];  T.m[8] = m[8];
			return T;
		}
		
		Matrix3x3(){
			m[0] = 1.f; m[3] = 0.f; m[6] = 0.f;
			m[1] = 0.f; m[4] = 1.f; m[7] = 0.f;
			m[2] = 0.f; m[5] = 0.f; m[8] = 1.f;			
		}
		static Matrix3x3 translation(float tx, float ty) {
			return {
				1.f, 0.f, 0.f,
				0.f, 1.f, 0.f,
				tx , ty , 1.f
			};
		}
		static Matrix3x3 scale(float sx, float sy) {
			return {
				sx , 0.f, 0.f,
				0.f, sy , 0.f,
				0.f, 0.f, 1.f
			};
		}
		static Matrix3x3 rotation(float radians) {
			float c = std::cos(radians);
			float s = std::sin(radians);
			return {
				 c ,  s , 0.f,
				-s ,  c , 0.f,
				 0.f, 0.f, 1.f
			};
		}
		
		

		Matrix3x3 Matrix3x3::operator*(const Matrix3x3& B) const {
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