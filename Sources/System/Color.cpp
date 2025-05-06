#include <System/Color.h>
#include <System/Mathf.hpp>
namespace System {
    const Color Color::black(0.0f, 0.0f, 0.0f, 1.0f);
    const Color Color::white(1.0f, 1.0f, 1.0f, 1.0f);



    System::Color::Color() {
        this->r = 0.0f;
        this->g = 0.0f;
        this->b = 0.0f;
        this->a = 0.0f;
    }
    System::Color::Color(float new_r, float new_g, float new_b, float new_a) {
        this->r = new_r;
        this->g = new_g;
        this->b = new_b;
        this->a = new_a;
    }
    Color Color::Lerp(const Color lhs, const Color rhs, const float t) {
        return LerpUnclamped(lhs,rhs,System::Mathf::Clamp(t, 0.0f, 1.0f));
    }
    Color Color::LerpUnclamped(const Color lhs, const Color rhs, const float t){
        return Color(
            lhs.r + t * (rhs.r - lhs.r),
            lhs.g + t * (rhs.g - lhs.g),
            lhs.b + t * (rhs.b - lhs.b),
            lhs.a + t * (rhs.a - lhs.a)
        );
    }

}