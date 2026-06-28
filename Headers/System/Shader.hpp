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
        std::string name;




        Shader() : ID(0) {}
       // Shader(const char* vertexPath, const char* fragmentPath);
        //
       // Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
        static Shader* CreateFromSource(const char* vertexSource, const char* fragmentSource);
        static Shader* CreateFromSource(const char* vertexSource, const char* fragmentSource, const char* geometrySource);

        static Shader* CreateFromStrings(std::string shadername, std::string vertexSource, std::string fragmentSource);
        static Shader* CreateFromStrings(std::string shadername, std::string vertexSource, std::string fragmentSource, std::string geometrySource);

        static Shader* CreateFromFiles(std::string shadername, std::string vertexPath, std::string fragmentPath);
        static Shader* CreateFromFiles(std::string shadername, std::string vertexPath, std::string fragmentPath, std::string geometryPath);

        void use() const {
            System::Graphics::GL::gl_glUseProgram(ID);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="tofind"></param>
        /// <returns></returns>
      //  static Shader* Find(std::string tofind) {
       //     return nullptr;
       // }



    private:
        //only MeshRenderer can set uniforms, to avoid accidental misuse
        friend class MeshRenderer;
        void setBool(const std::string& name, bool value) const {
            System::Graphics::GL::gl_glUniform1i(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string& name, int value) const {
            System::Graphics::GL::gl_glUniform1i(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string& name, float value) const {
            System::Graphics::GL::gl_glUniform1f(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), value);
        }
        void setVec3(const std::string& name, const Vector3& value) const {
            System::Graphics::GL::gl_glUniform3fv(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), 1, &value.x);
        }
        void setMat4(const std::string& name, const Matrix4x4& mat) const {
            System::Graphics::GL::gl_glUniformMatrix4fv(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), 1, false, &mat.m00);
        }
        void setColor(const std::string& name, const Color& color) const {
            System::Graphics::GL::gl_glUniform4f(System::Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), color.r, color.g, color.b, color.a);
        }


        static std::string ReadFile(const char* path);
        static unsigned int CompileShader(const char* source, System::Graphics::shaderTypes type);
        static unsigned int LinkProgram(unsigned int vertex, unsigned int fragment, unsigned int geometry = 0);

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