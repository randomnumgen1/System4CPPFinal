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



        Color32();

        Color32(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a);


        


        

    };
    inline bool operator==(const Color32& lhs, const Color32& rhs) {
        return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
    }

    inline bool operator!=(const Color32& lhs, const Color32& rhs) {
        return !(lhs == rhs);
    }
}

#endif