#ifndef _SYSTEM_COLOR_H
#define _SYSTEM_COLOR_H 

namespace System {
    class Color {
        static const Color black;
        static const Color white;


        float r, g, b, a;
    
        Color();
        Color(float r, float g, float b, float a);
    

        static Color Lerp(Color a, Color b, float t);
        static Color LerpUnclamped(Color a, Color b, float t);

    };
}

#endif