#include <System/Color32.hpp>
#include <System/Mathf.hpp>
namespace System {
    const Color32 Color32::black(0, 0, 0, 255);
    const Color32 Color32::white(255, 255, 255, 255);
    const Color32 Color32::red(255,  0,  0,255);
    const Color32 Color32::green(0, 128, 0, 255);
    const Color32 Color32::blue(0, 0, 255, 255);


    Color32::Color32(){
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }

    Color32::Color32(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a){
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    Color32 Color32::Lerp(const Color32 lhs, const Color32 rhs, float t){
        t = Mathf::Clamp01(t);
        return LerpUnclamped(lhs, rhs,t);
    }

    Color32 Color32::LerpUnclamped(const Color32 lhs, const Color32 rhs, const float t){
        return Color32(
            static_cast<uint8_t>(lhs.r + (rhs.r - lhs.r) * t),
            static_cast<uint8_t>(lhs.g + (rhs.g - lhs.g) * t),
            static_cast<uint8_t>(lhs.b + (rhs.b - lhs.b) * t),
            static_cast<uint8_t>(lhs.a + (rhs.a - lhs.a) * t)
        );
    }


}


 
 