
#ifndef _SYSTEM_MATERIAL_HPP
#define _SYSTEM_MATERIAL_HPP

 

#include <System/Shader.hpp>
#include <System/Texture2d.hpp>
#include <System/Color.hpp>
#include <System/Vector2.hpp>
#include <System/Vector3.hpp>
#include <System/Vector4.hpp>
#include <System/Matrix4x4.hpp>
#include <map>
#include <string>









namespace System {
    class Material {
    public:
        Shader* shader;
        Color color;
        Texture2D* mainTexture;

        std::map<std::string, float> floats;
        std::map<std::string, Vector2> vector2s;
        std::map<std::string, Vector3> vector3s;
        std::map<std::string, Vector4> vector4s;
        std::map<std::string, Matrix4x4> matrix4x4s;
        std::map<std::string, uint32_t> textureIds;


        Material(Shader* shader) : shader(shader), color(1.0f, 1.0f, 1.0f, 1.0f), mainTexture(nullptr) {}

        void setFloat(const std::string& name, float value) {
            floats[name] = value;
        }

        void SetTexture1(std::string name, System::Texture2D value) {
            //textureIds[name] = value.;
        }
       void setTexture2(const std::string& name, uint32_t textureId) {
            textureIds[name] = textureId;
        }
        void setVector2(const std::string& name, Vector2 value) {
            vector2s[name] = value;
        }
        void setVector3(std::string name, System::Vector3 value){
            vector3s[name] = value;
        }
        void setVector4(const std::string& name, Vector4 value) {
            vector4s[name] = value;
        }
        void setMatrix(const std::string& name, Matrix4x4 value) {
            matrix4x4s[name] = value;
        }
        void BindProperties() {
            if (!shader) return;

            uint32_t program = shader->ID;
           // Graphics::GL::gl_glUseProgram(program);

            // Bind built-in properties
            Graphics::GL::gl_glUniform4f(Graphics::GL::gl_glGetUniformLocation(program, "color"), color.r, color.g, color.b, color.a);

            // Bind float properties
            for (auto const& [name, value] : floats) {
                Graphics::GL::gl_glUniform1f(Graphics::GL::gl_glGetUniformLocation(program, name.c_str()), value);
            }

            // Bind Vector2 properties
            for (auto const& [name, value] : vector2s) {
               // Graphics::GL::gl_glUniform2f(Graphics::GL::gl_glGetUniformLocation(program, name.c_str()), value.x, value.y);
            }

            // Bind Vector3 properties
            for (auto const& [name, value] : vector3s) {
                Graphics::GL::gl_glUniform3fv(Graphics::GL::gl_glGetUniformLocation(program, name.c_str()), 1, &value.x);
            }

            // Bind Vector4 properties
            for (auto const& [name, value] : vector4s) {
                Graphics::GL::gl_glUniform4f(Graphics::GL::gl_glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z, value.w);
            }

            // Bind Matrix properties
            for (auto const& [name, value] : matrix4x4s) {
                Graphics::GL::gl_glUniformMatrix4fv(Graphics::GL::gl_glGetUniformLocation(program, name.c_str()), 1, false, value.raw);
            }

            // Bind texture properties
            int textureUnit = 1; // 0 is usually mainTexture
            if (mainTexture) {
               Graphics::GL::gl_glActiveTexture(0x84C0); // GL_TEXTURE0
               Graphics::GL::gl_glBindTexture(Graphics::GLenum1::GL_TEXTURE_2D, mainTexture->textureID);
                 Graphics::GL::gl_glUniform1i(Graphics::GL::gl_glGetUniformLocation(program, "mainTexture"), 0);
            }

            for (auto const& [name, textureId] : textureIds) {
                Graphics::GL::gl_glActiveTexture(static_cast<GLenum>(0x84C0 + textureUnit)); // GL_TEXTURE0 + unit
                Graphics::GL::gl_glBindTexture(Graphics::GLenum1::GL_TEXTURE_2D, textureId);
                Graphics::GL::gl_glUniform1i(Graphics::GL::gl_glGetUniformLocation(program, name.c_str()), textureUnit);
                textureUnit++;
            }
        }
 
    };
}

#endif