#include <System/Mathf.h>
#include <cmath>
#include <algorithm>
#include <stdexcept>
/*
----------------------------------------------------------------------------------------
Static Methods
----------------------------------------------------------------------------------------
*/
float System::Mathf::Abs(const float f){
    return std::fabsf(f);
}
float System::Mathf::Acos(const float f){
    return std::acosf(f);
}
bool System::Mathf::Approximately(const float a, const  float b){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::Asin(const float f){
    return std::asinf(f);
}
float System::Mathf::Atan(const float f){
    return std::atanf(f);
}
float System::Mathf::Atan2(const float y, const  float x){
    return std::atan2f(y,x);
}
float System::Mathf::Ceil(const float f){
    return std::ceilf(f);
}
int System::Mathf::CeilToInt(float f){
    return static_cast<int>(Ceil(f));
}
float System::Mathf::Clamp(float value, float min, float max){
    return std::clamp(value, min, max);
}
float System::Mathf::Clamp01(float value){
    return std::clamp(value, 0.0f, 1.0f);
}
int System::Mathf::ClosestPowerOfTwo(int value){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::Cos(float f){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::DeltaAngle(float current, float target){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::Exp(float power){
    throw std::runtime_error("Not Implemented Error.");
}
uint16_t System::Mathf::FloatToHalf(float val){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::Floor(float f){
    return std::floorf(f);
}
int System::Mathf::FloorToInt(float f){
    return static_cast<int>(Floor(f));
}
float System::Mathf::HalfToFloat(uint16_t val){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::InverseLerp(float a, float b, float value){
    if (a != b) {
        return Mathf::Clamp01((value - a) / (b - a));
    }else {
        return 0.0f;
    }
}
bool System::Mathf::IsPowerOfTwo(int val){
    return (val & (val - 1)) == 0;
}
float System::Mathf::Lerp(float a, float b, float t){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::LerpAngle(float a, float b, float t){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::LerpUnclamped(float a, float b, float t){
    return a + ((b - a) * t);
}
float System::Mathf::Log(float f, float p){
   return log(f) / log(p);
}
float System::Mathf::Log10(float f){
    return std::log10f(f);
}
float System::Mathf::Max(float a, float b){
   return std::fmaxf(a, b);
}
float System::Mathf::Min(float a, float b){
    return std::fminf(a,b);
}
float System::Mathf::MoveTowards(float current, float target, float maxDelta){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::MoveTowardsAngle(float current, float target, float maxDelta){
    throw std::runtime_error("Not Implemented Error.");
}
int System::Mathf::NextPowerOfTwo(int value){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::PerlinNoise(float x, float y){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::PerlinNoise1D(float x){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::PingPong(float t, float length){
   return length - Mathf::Abs(Mathf::Repeat(t, length * 2.0f) - length);
}
float System::Mathf::Pow(float base, float exp){
    return std::pow(base, exp);
}
float System::Mathf::Repeat(float t, float length){
    return Clamp(t - Mathf::Floor(t / length) * length, 0.0f, length);
}
float System::Mathf::Round(float f){
    return std::roundf(f);
}
int System::Mathf::RoundToInt(float f){
    return static_cast<int>(Round(f));
}
float System::Mathf::Sign(float f){
    return f >= 0.0f ? 1.0f : -1.0f;
}
float System::Mathf::Sin(float f){
    return std::sinf(f);
}
float System::Mathf::SmoothStep(float from, float to, float t){
    throw std::runtime_error("Not Implemented Error.");
}
float System::Mathf::Sqrt(float f){
    return std::sqrtf(f);
}
float System::Mathf::Tan(float f){
    return std::tanf(f);
}
