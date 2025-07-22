#ifndef _SYSTEM_COLOR32_H
#define _SYSTEM_COLOR32_H

#include <cassert>
#include <cstdint>
namespace System {
    struct Color32 {
    public:
        uint8_t r, g, b, a;


        static const Color32 black;
        static const Color32 white;
        static const Color32 red;
        static const Color32 green;
        static const Color32 blue;



        Color32() {
            r = 0;
            g = 0;
            b = 0;
            a = 0;
        }

        Color32(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) {
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }


        static Color32 Lerp(const Color32 lhs, const Color32 rhs, const float t) {}
        static Color32 LerpUnclamped(const Color32 lhs, const Color32 rhs, const float t) {}


        

    };
}

#endif