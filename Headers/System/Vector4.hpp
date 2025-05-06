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
       /*  
        ----------------------------------------------------------------------------------------  
       Constructors  
        ----------------------------------------------------------------------------------------  
       */  
       Vector4();  
       Vector4(float x, float y, float z, float w);  
       /*  
       ----------------------------------------------------------------------------------------  
       Static Methods  
       ----------------------------------------------------------------------------------------  
       */  
       static float Distance(Vector4 a, Vector4 b);
       static float Dot(Vector4 a, Vector4 b);
       static Vector4 Lerp(Vector4 a, Vector4 b, float t);
       static Vector4 LerpUnclamped(Vector4 a, Vector4 b, float t);
       static Vector4 Max(Vector4 lhs, Vector4 rhs);
       static Vector4 Min(Vector4 lhs, Vector4 rhs);
       static Vector4 MoveTowards(Vector4 current, Vector4 target, float maxDistanceDelta);
       static Vector4 Project(Vector4 a, Vector4 b);
       static Vector4 Scale(Vector4 a, Vector4 b);
   };  
}  
#endif // _SYSTEM_VECTOR4_H