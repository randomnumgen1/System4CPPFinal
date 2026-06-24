
#ifndef _SYSTEM_MATERIAL_HPP
#define _SYSTEM_MATERIAL_HPP

#include <System/Shader.hpp>
#include <System/Texture2d.hpp>
#include <System/Color.hpp>
#include <System/Vector3.hpp>
#include <map>
#include <string>

namespace System {
    class Material {
    public:
        Shader* shader;
        Color color;
        Texture2D* mainTexture;

        std::map<std::string, float> floats;
        std::map<std::string, uint32_t> textureIds;
        std::map<std::string, System::Vector3> vector3s;


        Material(Shader* shader) : shader(shader), color(1.0f, 1.0f, 1.0f, 1.0f), mainTexture(nullptr) {}

        void setFloat(const std::string& name, float value) {
            floats[name] = value;
        }

        void SetTexture(std::string name, System::Texture2D value) {
            //textureIds[name] = value.;
        }
        void setVector3(std::string name, System::Vector3 value){
            vector3s[name] = value;
        }
    };
}

#endif