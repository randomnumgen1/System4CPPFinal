#ifndef _SYSTEM_SHADER_HPP
#define _SYSTEM_SHADER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <System/Graphics/GraphicsHelpers.h>
#include <System/Matrix4x4.hpp>
#include <System/Color.hpp>

namespace System { 
    class Shader {
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);

        void use() {
            System::Graphics::GL::gl_glUseProgram(ID);
        }
        void setBool(const std::string& name, bool value) {
            System::Graphics::GL::gl_glUniform1i(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string& name, int value) {
            System::Graphics::GL::gl_glUniform1i(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string& name, float value) {
            System::Graphics::GL::gl_glUniform1f(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), value);
        }
        void setVec3(const std::string& name, const Vector3& value) {
            System::Graphics::GL::gl_glUniform3fv(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), 1, &value.x);
        }
        void setMat4(const std::string& name, const Matrix4x4& mat) {
            System::Graphics::GL::gl_glUniformMatrix4fv(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), 1, false, &mat.m00);
        }
        void setColor(const std::string& name, const Color& color) {
            System::Graphics::GL::gl_glUniform4f(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), color.r, color.g, color.b, color.a);
        }
    private:
        void checkCompileErrors(unsigned int shader, std::string type) {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM") {
                System::Graphics::GL::gl_glGetShaderiv(shader, System::Graphics::ShaderParam::COMPILE_STATUS, &success);
                if (!success) {
                    System::Graphics::GL::gl_glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else {
                System::Graphics::GL::gl_glGetProgramiv(shader, System::Graphics::ProgramParam::LINK_STATUS, &success);
                if (!success) {
                    System::Graphics::GL::gl_glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
    };
}

#endif