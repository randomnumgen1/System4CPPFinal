#ifndef _SYSTEM_COLOR_H
#define _SYSTEM_COLOR_H

#include <cassert>
namespace System {
    struct Color {
    public:
        float r, g, b, a;

        static const Color black;
        static const Color white;

       
    
        Color();
        Color( float r, float g, float b, float a);
    

        static Color Lerp(const Color lhs, const Color rhs, const float t);
        static Color LerpUnclamped(const Color lhs, const Color rhs, const float t);


        inline float operator[](int i) const { assert((i >= 0) && (i < 3)); return (&r)[i]; }
        inline float& operator[](int i) { assert((i >= 0) && (i < 3)); return (&r)[i]; }

    };
}

#endif