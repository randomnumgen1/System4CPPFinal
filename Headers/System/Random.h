#ifndef _SYSTEM_RANDOM_H
#define _SYSTEM_RANDOM_H

#include <cstdint>
#include <System/Mathf.h>
#include <System/Vector2.hpp>
/*
 Random.h
 IMPORTANT: DO NOT USE THIS FOR SECURITY OR CRYPTOGRAPHY
 This is a simple random number generator using the Xorshift 128 algorithm.

 It generates the same random numbers on all platforms if you use the same starting seed.
 This makes it useful for things like procedural world generation in games.

 For example:
 - If the seed is the same, calling NextInt once will return 5 on all platforms.
 - Calling NextInt again will return 12 on all platforms.

 This means you can carefully use it to make sure objects in your game are placed the same way across different platforms.
 */
namespace System{
	struct Random{
	public:
		static uint32_t  m_seed[4];
		static uint32_t& x() { return m_seed[0]; }
		static uint32_t& y() { return m_seed[1]; }
		static uint32_t& z() { return m_seed[2]; }
		static uint32_t& w() { return m_seed[3]; }
 		static void InitState(const int seed){
			m_seed[0] = seed;
			m_seed[1] = m_seed[0] * 0x6c078965 + 1;
			m_seed[2] = m_seed[1] * 0x6c078965 + 1;
			m_seed[3] = m_seed[2] * 0x6c078965 + 1;
		}
		static uint32_t XORShift() {
			uint32_t t = x() ^ (x() << 11);

			x() = y(); 
			y() = z(); 
			z() = w();

			w() = w() ^ (w() >> 19) ^ t ^ (t >> 8);

			return w();

		}
		static uint32_t NextInt() {
			uint32_t mut1 = (m_seed[0] << 11) ^ m_seed[0];

			m_seed[0] = m_seed[1];
			m_seed[1] = m_seed[2];
			m_seed[2] = m_seed[3];

			mut1 = (((m_seed[3] >> 11) ^ mut1) >> 8) ^ m_seed[3] ^ mut1;

			m_seed[3] = mut1;

			return mut1;
		}

		static float value() {
			return ((float)(NextInt() & 0x7FFFFF)) * 1.192093e-07f;
		}
		static float value2() {
			return ((float)(XORShift() & 0x7FFFFF)) * 1.192093e-07f;
		}
		static int Range(const int minInclusive, const int maxExclusive){
			if (maxExclusive - minInclusive == 0) return minInclusive;
			int64_t minLong = (int64_t)minInclusive;
			int64_t maxLong = (int64_t)maxExclusive;
			int64_t r = XORShift();
			if (maxExclusive < minInclusive)
				return (int)(minLong - r % (maxLong - minLong));
			else
				return (int)(minLong + r % (maxLong - minLong));
		}
		static float Range(const float minInclusive, const float maxInclusive) {
			// Assume value() returns [0,1]
			float result = value2();
			return (1.0f - result) * maxInclusive + result * minInclusive;
		}
#ifdef _SYSTEM_EXPERIIMENTAL 

		static System::Vector2 InsideUnitCircle(){
			float rad = Range(0.0f, Mathf::PI * 2.0f);
			float x = Mathf::Cos(rad);
			float y = Mathf::Sin(rad);
			float ze = Range(0.0f, 1.0f);
			float d = Mathf::Sqrt(ze);
			return Vector2(x * d, y * d);
		}
#endif	 
	};
	uint32_t  System::Random::m_seed[4] = {0,0,0,0};
}
#endif