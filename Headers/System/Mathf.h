#ifndef _SYSTEM_MATHF_H
#define _SYSTEM_MATHF_H

namespace System{
	struct Mathf{
		public:
		static float Abs(float f);
		static float Acos(float f);
		static bool Approximately(float a, float b);
		static float Asin(float f);
		static float Atan(float f);
		static float Atan2(float y, float x);
		static float Ceil(float f);
		static int CeilToInt(float f);
		static float Clamp(float value, float min, float max);
		static float Clamp01(float value);
		static int ClosestPowerOfTwo(int value);
		static float Cos(float f);
		static float DeltaAngle(float current, float target);
		static float Exp(float power);
		static uint16_t FloatToHalf(float val);
		static float Floor(float f);
		static int FloorToInt(float f);
		static float HalfToFloat(uint16_t val);
		
	};
}
#endif