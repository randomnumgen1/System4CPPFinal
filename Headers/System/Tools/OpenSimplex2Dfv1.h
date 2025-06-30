#ifndef SYSTEM_TOOLS_OPENSIMPLEX2DFV1
#define SYSTEM_TOOLS_OPENSIMPLEX2DFV1

#include <cmath>
#include <array>

namespace System::Tools {

class OpenSimplex2Dfv1 {
public:
    explicit OpenSimplex2D(int seed = 0){
		std::array<short, 256> source;
		std::iota(source.begin(), source.end(), 0);

		std::mt19937 rng(seed);
		std::shuffle(source.begin(), source.end(), rng);

		for (int i = 0; i < 256; ++i) {
			perm[i] = source[i];
			perm2D[i] = perm[i] % 16;
		}
	}
    float Noise(float x, float y) const{
		// Stretch input space
		float stretchOffset = (x + y) * STRETCH;
		float xs = x + stretchOffset;
		float ys = y + stretchOffset;

		int xsb = static_cast<int>(std::floor(xs));
		int ysb = static_cast<int>(std::floor(ys));

		float squishOffset = (xsb + ysb) * SQUISH;
		float dx0 = x - (xsb + squishOffset);
		float dy0 = y - (ysb + squishOffset);

		float dx1 = dx0 - 1 - SQUISH;
		float dy1 = dy0 - SQUISH;
		float dx2 = dx0 - SQUISH;
		float dy2 = dy0 - 1 - SQUISH;

		float attn0 = 2.0f - dx0 * dx0 - dy0 * dy0;
		float attn1 = 2.0f - dx1 * dx1 - dy1 * dy1;
		float attn2 = 2.0f - dx2 * dx2 - dy2 * dy2;

		float value = 0.0f;
		if (attn0 > 0) {
			attn0 *= attn0;
			value += attn0 * attn0 * Extrapolate(xsb, ysb, dx0, dy0, perm);
		}
		if (attn1 > 0) {
			attn1 *= attn1;
			value += attn1 * attn1 * Extrapolate(xsb + 1, ysb, dx1, dy1, perm);
		}
		if (attn2 > 0) {
			attn2 *= attn2;
			value += attn2 * attn2 * Extrapolate(xsb, ysb + 1, dx2, dy2, perm);
		}

		return value * 45.23065f; // Normalize to ~[-1,1]
	}
private:
    std::array<short, 256> perm;
    std::array<short, 256> perm2D;

    static constexpr float STRETCH = -0.211324865f; // (1/√2-1)/2
    static constexpr float SQUISH  = 0.366025403f;  // (√2-1)/2

    static float Extrapolate(int xsb, int ysb, float dx, float dy, const std::array<short, 256>& perm) {
		int index = perm[(perm[xsb & 0xFF] + ysb) & 0xFF] & 0x0E;
		float gx = gradients2D[index];
		float gy = gradients2D[index + 1];
		return gx * dx + gy * dy;
	}
    static constexpr int gradients2D[16] = {
        5,  2,  2,  5,
       -5,  2, -2,  5,
        5, -2,  2, -5,
       -5, -2, -2, -5
    };
};

}
#endif