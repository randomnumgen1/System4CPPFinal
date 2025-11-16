#ifndef _SYSTEM_VECTOR3_H
#define _SYSTEM_VECTOR3_H

#include <assert.h>
 
//#include <System/Vector2.h>

namespace System {
    struct Vector3 {
    public:
        union {
            float v[3];
            struct {
                float x;
                float y;
                float z;
            };
        };
       
 
        static constexpr float kEpsilon = 0.00001F;
        static constexpr float kEpsilonNormalSqrt = 1e-15F;


        static const Vector3 back;
        static const Vector3 down;
        static const Vector3 forward;
        static const Vector3 left;
        static const Vector3 one;
        static const Vector3 right;
        static const Vector3 up;
        static const Vector3 zero;


        /*
         ----------------------------------------------------------------------------------------
        Constructors
         ----------------------------------------------------------------------------------------
        */
      
        Vector3();
        Vector3(float x, float y, float z);

        /*inline explicit Vector3( System::Vector2 lol) {
            this->x = lol.x;
            this->y = lol.y;
            this->z = 0.0f;
        }
        *  
        * 
        * 
        ----------------------------------------------------------------------------------------
        Static Methods
        ----------------------------------------------------------------------------------------
        */
        static float Angle(const Vector3 from, const  Vector3 to);
		static Vector3 ClampMagnitude(const Vector3 vector, const  float maxLength);
        static Vector3 Cross(const Vector3 lhs, const  Vector3 rhs);
        static float Distance(const Vector3 lhs, const  Vector3 rhs);
        static float Dot(const Vector3 lhs, const  Vector3 rhs);
        /// <summary>
        /// Performs linear interpolation between two vectors based on a blend factor.
        /// </summary>
        /// <param name="lhs">The start vector (returned when t = 0).</param>
        /// <param name="rhs">The end vector (returned when t = 1).</param>
        /// <param name="t">Interpolation factor (0 returns lhs,.5 is between lhs and rhs, .1 returns rhs).</param>
        /// <returns></returns>
        static Vector3 Lerp(const Vector3 lhs, const  Vector3 rhs,const float t);
        /// <summary>
        /// Performs linear interpolation between two vectors based on a blend factor, without clamping the factor to the [0, 1] range.
        /// </summary>
        /// <param name="lhs">The start vector.</param>
        /// <param name="rhs">The end vector.</param>
        /// <param name="t">Interpolation factor.</param>
        /// <returns>The interpolated vector.</returns>
        static Vector3 LerpUnclamped(const Vector3 lhs, const  Vector3 rhs, const  float t);
        /// <summary>
        /// Computes a component-wise maximum between two 3D vectors.
        /// For each axis (X, Y, Z), selects the greater (or lesser) value from the inputs.
        /// </summary>
        /// <param name="lhs">The first input vector.</param>
        /// <param name="rhs">The second input vector.</param>
        /// <returns>A new vector containing the maximum components from <paramref name="lhs"/> and <paramref name="rhs"/>.</returns>
        static Vector3 Max(const Vector3 lhs, const  Vector3 rhs) noexcept;
        /// <summary>
        /// Computes a component-wise minimum between two 3D vectors.
        /// For each axis (X, Y, Z), selects the lesser value from the inputs.
        /// </summary>
        /// <param name="lhs">The first input vector.</param>
        /// <param name="rhs">The second input vector.</param>
        /// <returns>A new vector containing the minimum components from <paramref name="lhs"/> and <paramref name="rhs"/>.</returns>
        static Vector3 Min(const Vector3 lhs, const  Vector3 rhs) noexcept;
        static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta);
        /// <summary>
        /// This does not modify the given vector.
        /// </summary>
        /// <param name="value"></param>
        /// <returns>A new vector with the same direction as the original vector but with a magnitude of 1.0.</returns>
        static Vector3 Normalize(const Vector3 value);
        static void OrthoNormalize(Vector3& normal, Vector3& tangent);
        static Vector3 Project(const Vector3 vector, const  Vector3 onNormal);
        static Vector3 ProjectOnPlane(const Vector3 vector, const  Vector3 planeNormal);
        static Vector3 Reflect(const Vector3 inDirection, const  Vector3 inNormal);
        static Vector3 RotateTowards(Vector3 current, Vector3 target, float maxRadiansDelta, float maxMagnitudeDelta);
        static Vector3 Scale(const Vector3 a, const  Vector3 b);
        static float SignedAngle(Vector3 from, Vector3 to, Vector3 axis);
        static Vector3 Slerp(Vector3 a, Vector3 b, float t);
        static Vector3 SlerpUnclamped(Vector3 lhs, Vector3 rhs, float t);
        static Vector3 SmoothDamp();
        /*
        ----------------------------------------------------------------------------------------
        Normal Methods
        ----------------------------------------------------------------------------------------
        */
        float magnitude() const noexcept;
        float sqrMagnitude() const noexcept;
        /// <summary>
        /// Sets the x, y and z components of the vector.
        /// </summary>
        /// <param name="new_x">The new x-component.</param>
        /// <param name="new_y">The new y-component.</param>
        /// <param name="new_z">The new z-component.</param>
        void Set(float new_x, float new_y, float new_z);
        /// <summary>
        /// Makes this vector have a magnitude of 1.
        /// **this function changes the current vector**
        /// </summary>
        void Normalize();
        /// <summary>
        /// This does not modify the given vector.
        /// </summary>
        /// <returns></returns>
        Vector3 normalized();
        /*
        ----------------------------------------------------------------------------------------
        Operators
        ----------------------------------------------------------------------------------------
        */

        Vector3 operator*(float scalar) const {
            return Vector3{
                x * scalar,
                y * scalar,
                z * scalar
            };
        }
        Vector3 operator/(float scalar) const {
            return Vector3{
                x / scalar,
                y / scalar,
                z / scalar
            };
        }
        Vector3 operator*(const Vector3& other) const {
            return Vector3{
                x * other.x,
                y * other.y,
                z * other.z
            };
        }
        Vector3 operator/(const Vector3& other) const {
            return Vector3{
                x / other.x,
                y / other.y,
                z / other.z
            };
        }
        Vector3 operator-(const Vector3& other) const {
            return Vector3{
                x - other.x,
                y - other.y,
                z - other.z
            };
        }   
        Vector3& operator-=(const Vector3& other) {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }
        Vector3 operator+(const Vector3& other) const {
            return Vector3{
                x + other.x,
                y + other.y,
                z + other.z
            };
        }
        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }
        bool operator==(const Vector3& rhs) const { 
            float diff_x = x - rhs.x;
            float diff_y = y - rhs.y;
            float diff_z = z - rhs.z;
            float sqrmag = diff_x * diff_x + diff_y * diff_y + diff_z * diff_z;
            return sqrmag < kEpsilon * kEpsilon;
        }
        bool operator!=(const Vector3& rhs) const {
            return !(*this == rhs);
        }





        inline float operator[](int i) const {assert((i >= 0) && (i < 3));return (&x)[i];}
        inline float& operator[](int i) { assert((i >= 0) && (i < 3));return (&x)[i]; }





    };

}
#endif