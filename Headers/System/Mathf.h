#include <cstdint>
#ifndef _SYSTEM_MATHF_H
#define _SYSTEM_MATHF_H

namespace System{
	struct Mathf{
		public:
		/*
		----------------------------------------------------------------------------------------
		Static Methods
		----------------------------------------------------------------------------------------
		*/
		static float Abs(const float f);
		static float Acos(const float f);
		static bool Approximately(const float a, const  float b);
		static float Asin(const float f);
		static float Atan(const float f);
		static float Atan2(const float y, const  float x);
		static float Ceil(const float f);
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
		static float InverseLerp(float a, float b, float value);
		static bool IsPowerOfTwo(int value);
		static float Lerp(float a, float b, float t);
		static float LerpAngle(float a, float b, float t);
		static float LerpUnclamped(float a, float b, float t);
		static float Log(float f, float p);
		static float Log10(float f);
		static float Max(float a, float b);
		static float Min(float a, float b);
		static float MoveTowards(float current, float target, float maxDelta);
		static float MoveTowardsAngle(float current, float target, float maxDelta);
		static int NextPowerOfTwo(int value);
		static float PerlinNoise(float x, float y);
		static float PerlinNoise1D(float x);
		static float PingPong(float t, float length);
		static float Pow(float f, float p);
		static float Repeat(float t, float length);
		static float Round(float f);
		static int RoundToInt(float f);
		static float Sign(float f);
		static float Sin(float f);
		static float SmoothStep(float from, float to, float t);
		static float Sqrt(float f);
		static float Tan(float f);

	};
}
#endif