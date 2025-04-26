#include <System/Mathf.h>
#include <cmath>
#include <algorithm>
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
    return false;
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
    return 0;
}
float System::Mathf::Clamp(float value, float min, float max){
    return std::clamp(value, min, max);
}
float System::Mathf::Clamp01(float value){
    return std::clamp(value, 0.0f, 1.0f);
}
int System::Mathf::ClosestPowerOfTwo(int value){
    return 0;
}
float System::Mathf::Cos(float f){
    return 0.0f;
}
float System::Mathf::DeltaAngle(float current, float target){
    return 0.0f;
}
float System::Mathf::Exp(float power){
    return 0.0f;
}
uint16_t System::Mathf::FloatToHalf(float val){
    return 0;
}
float System::Mathf::Floor(float f){
    return std::floorf(f);
}
int System::Mathf::FloorToInt(float f){
    return 0;
}
float System::Mathf::HalfToFloat(uint16_t val){
    return 0.0f;
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
    return 0.0f;
}
float System::Mathf::LerpAngle(float a, float b, float t){
    return 0.0f;
}
float System::Mathf::LerpUnclamped(float a, float b, float t){
    return 0.0f;
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
    return 0.0f;
}
float System::Mathf::MoveTowardsAngle(float current, float target, float maxDelta){
    return 0.0f;
}
int System::Mathf::NextPowerOfTwo(int value){
    return 0;
}
float System::Mathf::PerlinNoise(float x, float y){
    return 0.0f;
}
float System::Mathf::PerlinNoise1D(float x){
    return 0.0f;
}
float System::Mathf::PingPong(float t, float length){
   return length - Mathf::Abs(Mathf::Repeat(t, length * 2.0f) - length);
}
float System::Mathf::Pow(float f, float p){
    return 0.0f;
}
float System::Mathf::Repeat(float t, float length){
    return Clamp(t - Mathf::Floor(t / length) * length, 0.0f, length);
}
float System::Mathf::Round(float f){
    return std::roundf(f);
}
int System::Mathf::RoundToInt(float f){
    return 0;
}
float System::Mathf::Sign(float f){
    return f >= 0.0f ? 1.0f : -1.0f;
}
float System::Mathf::Sin(float f){
    return std::sinf(f);
}
float System::Mathf::SmoothStep(float from, float to, float t){
    return 0.0f;
}
float System::Mathf::Sqrt(float f){
    return std::sqrtf(f);
}
float System::Mathf::Tan(float f){
    return std::tanf(f);
}
