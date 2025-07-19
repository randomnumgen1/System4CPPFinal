#ifndef _SYSTEM_MATHF_H
#define _SYSTEM_MATHF_H

#include <cstdint>

#ifdef _WIN32
    #include <corecrt_math_defines.h>
#else
	#define _USE_MATH_DEFINES
	#include <cmath>
#endif

namespace System{
	struct Mathf{
		public:
			static constexpr float PI =  static_cast<float>(M_PI);
			static constexpr float Deg2Rad = static_cast<float>(M_PI) / 180.0f; 
			static constexpr float Rad2Deg = 180.0f / static_cast<float>(M_PI);
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
		static int CountSetBits(uint32_t x);
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
		static uint64_t Majority3(uint64_t a, uint64_t b, uint64_t c);
		static uint64_t Majority5 (uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e);
		static float Map(const float originalNumber, const float oldMin, const float oldMax, const float newMin, const float newMax);
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
		static float SmoothStep(const float from, const float to, float t);
		static float Sqrt(float f);
		static float Tan(float f);
		static float Radians(float degrees);
		static float Degrees(float radians);
		/*
		 ----------------------------------------------------------------------------------------
		Tween In
		 ----------------------------------------------------------------------------------------
		*/
		static float TweenCircIn(float from, float to, float t) {
			return from + (to - from) * (1 - Sqrt(1 - t * t));
		}
		static float TweenExpoIn(float from, float to, float t) {
			return from + (to - from) * (t == 0 ? 0 : Pow(2, 10 * (t - 1)));
		}
		static float TweenQuintIn(float from, float to, float t) {
			return from + (to - from) * (t * t * t * t * t);
		}
		static float TweenQuartIn(float from, float to, float t) {
			return from + (to - from) * (t * t * t * t);
		}
		static float TweenCubicIn(float from, float to, float t) {
			return from + (to - from) * (t * t * t);
		}
		static float TweenQuadIn(float from, float to, float t) {
			return from + (to - from) * (t * t);
		}
		static float TweenSineIn(const float from, const float to, const float t) {
			return from + (to - from) * (1.0f - Cos((t * PI) / 2));
		}
		static float TweenBounceIn(float from, float to, float t) {
			return from + (to - from) * (1.0f - TweenBounceOut(0.0f, 1.0f, 1.0f - t));
		}
		static float TweenBackIn(const float from, const float to, const float t) {
			const float s = 1.70158f;
			return from + (to - from) * (t * t * ((s + 1) * t - s));
		}
		static float TweenElasticIn(float from, float to, float t) {
			if (t == 0.0f) return from;
			if (t == 1.0f) return to;
			return from + (to - from) * (-Pow(2.0f, 10.0f * (t - 1.0f)) * Sin((t * 10.0f - 0.75f) * (2.0f * PI) / 3.0f));
		}
		static float TweenEaseIn(const float from, const float to, const float t) {
			return from + (to - from) * (t * t);
		}
		/*
		 ----------------------------------------------------------------------------------------
		Tween Out
		 ----------------------------------------------------------------------------------------
		*/
		static float TweenCircOut(float from, float to, float t) {
			return from + (to - from) * Sqrt(1.0f - Pow(t - 1.0f, 2.0f));
		}
		static float TweenExpoOut(float from, float to, float t) {
			return from + (to - from) * (t == 1.0f ? 1.0f : 1.0f - Pow(2.0f, -10.0f * t));
		}
		static float TweenQuintOut(float from, float to, float t) {
			return from + (to - from) * (1.0f - Pow(1.0f - t, 5.0f));
		}
		static float TweenQuartOut(float from, float to, float t) {
			return from + (to - from) * (1.0f - Pow(1.0f - t, 4.0f));
		}
		static float TweenCubicOut(float from, float to, float t) {
			return from + (to - from) * (1.0f - Pow(1 - t, 3.0f));
		}
		static float TweenQuadOut(float from, float to, float t) {
			return from + (to - from) * (1.0f - (1.0f - t) * (1.0f - t));
		}
		static float TweenSineOut(const float from, const float to,const float t) {
			return from + (to - from) * Sin((t * PI) / 2.0f);
		}
		static float TweenBounceOut(float from, float to, float t) {
			if (t < 0.3636f) return from + (to - from) * (7.5625f * t * t);
			if (t < 0.7272f) return from + (to - from) * (7.5625f * (t -= 0.5454f) * t + 0.75f);
			if (t < 0.9090f) return from + (to - from) * (7.5625f * (t -= 0.8181f) * t + 0.9375f);
			return from + (to - from) * (7.5625f * (t -= 0.9545f) * t + 0.984375f);
		}
		static float TweenBackOut(float from, float to, float t) {
			const float s = 1.70158f;
			return from + (to - from) * (1 + s * Pow(t - 1.0f, 3.0f) + s * (t - 1) * (t - 1));
		}
		static float TweenElasticOut(float from, float to, float t) {
			if (t == 0.0f) return from;
			if (t == 1.0f) return to;
			return from + (to - from) * (Pow(2.0f, -10.0f * t) * Sin((t * 10.0f - 0.75f) * (2.0f * PI) / 3.0f) + 1.0f);
		}
		static float TweenEaseOut(const float from, const float to, const float t) {
			return from + (to - from) * (1 - (1 - t) * (1 - t));
		}




	};
}
#endif