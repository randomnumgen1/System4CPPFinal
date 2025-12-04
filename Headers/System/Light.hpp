#ifndef _SYSTEM_LIGHT_H
#define _SYSTEM_LIGHT_H



#include <iostream>
#include <System/Vector3.hpp>
#include <System/Quaternion.hpp>
#include <System/Matrix4x4.hpp>
#include <System/Component.hpp>
#include <System/Color32.hpp>
#include <vector>

namespace System {
    enum LightType {
        Spot,
        Directional,
        Point
    };
    class Light : public System::Component {
    private:
        
    public:
        static std::vector<Light*> allLights;

        LightType type;
        float range;
        Color32 color;
        float intensity;

        Component* Clone() const override {
            Light* newLight = new Light();
            newLight->type = type;
            newLight->range = range;
            newLight->color = color;
            newLight->intensity = intensity;
            return newLight;
        }

    };
}
#endif