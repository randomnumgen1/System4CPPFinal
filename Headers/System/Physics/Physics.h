#ifndef _SYSTEM_PHYSICS_H
#define _SYSTEM_PHYSICS_H

#include <System/Vector3.hpp>
#include <cstdint>
#include <System/Mathf.hpp>

namespace System{
    struct Ray {
    public:
        Vector3 origin;
        Vector3 direction;
        Ray(Vector3 origin, Vector3 direction){
            this->origin = origin;
            this->direction = direction;
        }
    };
    class Physics{
    public:
        static bool Raycast(Vector3 origin, Vector3 direction, float maxDistance = System::Mathf::Infinity, uint32_t layerMask = -1){
           
        }










    };
}
#endif