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

        /*
        ----------------------------------------------------------------------------------------
        Static Methods
        ----------------------------------------------------------------------------------------
        */
        static float Angle(Vector2 from, Vector2 to);
        static float Distance(const Vector2 lhs, const  Vector2 rhs);
        static float Dot(const Vector2 lhs, const  Vector2 rhs);
        static Vector2 Lerp(const Vector2 lhs, const  Vector2 rhs, float t);
        static Vector2 LerpUnclamped(const Vector2 lhs, const  Vector2 rhs, const  float t);
        static Vector2 Max(const Vector2 lhs, const  Vector2 rhs);
        static Vector2 Min(const Vector2 lhs, const  Vector2 rhs);
        static Vector2 Scale(const Vector2 a, const  Vector2 b);
        static float SignedAngle(Vector2 from, Vector2 to);
        void Set(float newX, float newY);

        float magnitude();
        float SqrMagnitude();




    };
}
#endif