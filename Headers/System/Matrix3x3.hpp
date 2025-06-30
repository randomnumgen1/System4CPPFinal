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
		

	};
}
#endif