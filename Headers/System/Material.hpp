#ifndef _SYSTEM_MATERIAL_HPP
#define _SYSTEM_MATERIAL_HPP

#include <System/Shader.hpp>
#include <System/Texture2d.hpp>
#include <System/Color.hpp>

namespace System {
    class Material {
    public:
        Shader* shader;
        Color color;
        Texture2d* mainTexture;

        Material(Shader* shader) : shader(shader), color(1.0f, 1.0f, 1.0f, 1.0f), mainTexture(nullptr) {}
    };
}

#endif