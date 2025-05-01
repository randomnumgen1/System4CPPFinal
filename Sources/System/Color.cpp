#include <System/Color.h>
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

}