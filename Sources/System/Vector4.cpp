#include <System/Vector4.h>
#include <System/Mathf.h>
#include <stdexcept>
System::Vector4::Vector4(){
    this->x = 0.0f; this->y = 0.0f; this->z = 0.0f; this->w = 0.0f;
}

System::Vector4::Vector4(float x, float y, float z, float w){
    this->x = x; this->y = y; this->z = z; this->w = w;
}
/*
----------------------------------------------------------------------------------------
Static Methods
----------------------------------------------------------------------------------------
*/
 

float System::Vector4::Distance(Vector4 a, Vector4 b){
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    float dw = a.w - b.w;
    return Mathf::Sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}

float System::Vector4::Dot(Vector4 a, Vector4 b){
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

System::Vector4 System::Vector4::Lerp(Vector4 a, Vector4 b, float t){
    return Vector4(
        a.x + t * (b.x - a.x),
        a.y + t * (b.y - a.y),
        a.z + t * (b.z - a.z),
        a.w + t * (b.w - a.w)
    );
}
System::Vector4 System::Vector4::LerpUnclamped(Vector4 a, Vector4 b, float t)  {  
   return Vector4(  
       a.x + t * (b.x - a.x),  
       a.y + t * (b.y - a.y),  
       a.z + t * (b.z - a.z),  
       a.w + t * (b.w - a.w)  
   );  
}  

System::Vector4 System::Vector4::Max(Vector4 lhs, Vector4 rhs)  {  
   return Vector4(  
       std::max(lhs.x, rhs.x),  
       std::max(lhs.y, rhs.y),  
       std::max(lhs.z, rhs.z),  
       std::max(lhs.w, rhs.w)  
   );  
}  

System::Vector4 System::Vector4::Min(Vector4 lhs, Vector4 rhs)  {  
   return Vector4(  
       std::min(lhs.x, rhs.x),  
       std::min(lhs.y, rhs.y),  
       std::min(lhs.z, rhs.z),  
       std::min(lhs.w, rhs.w)  
   );  
}  

System::Vector4 System::Vector4::MoveTowards(Vector4 current, Vector4 target, float maxDistanceDelta)  {  
    throw std::runtime_error("Not Implemented Error.");
}  

System::Vector4 System::Vector4::Project(Vector4 a, Vector4 b)  {  
    throw std::runtime_error("Not Implemented Error.");
}  

System::Vector4 System::Vector4::Scale(Vector4 a, Vector4 b)  {  
   return Vector4(  
       a.x * b.x,  
       a.y * b.y,  
       a.z * b.z,  
       a.w * b.w  
   );  
}

 

 
 
