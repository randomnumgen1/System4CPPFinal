#ifndef _SYSTEM_LIGHT_H
#define _SYSTEM_LIGHT_H



#include <iostream>
#include <System/Vector3.hpp>
#include <System/Quaternion.hpp>
#include <System/Matrix4x4.hpp>
#include <System/Component.h>
#include "Color32.hpp"

namespace System {
    enum LightType {
        Spot,
        Directional,
        Point
    };
    class Light : public System::Component {
    private:
        
    public:
        LightType type;
        float range;
        Color32 color;
        float intensity;

    };
}
#endif