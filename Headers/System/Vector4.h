#ifndef _SYSTEM_VECTOR4_H
#define _SYSTEM_VECTOR4_H
namespace System {
    struct Vector4 {
    public:
        union {
            float v[4];
            struct {
                float x;
                float y;
                float z;
                float w;
            };
        };

        Vector4(float x, float y, float z, float w) {
        
        }



    };
}
#endif