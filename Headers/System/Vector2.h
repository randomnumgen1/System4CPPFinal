#ifndef _SYSTEM_VECTOR2_H
#define _SYSTEM_VECTOR2_H
namespace System {
    struct Vector2 {
    public:
        union {
            float v[2];
            struct {
                float x;
                float y;
            };
        };
        /*
         ----------------------------------------------------------------------------------------
        Constructors
         ----------------------------------------------------------------------------------------
        */
        Vector2();
        Vector2(float x, float y);










    };
}
#endif