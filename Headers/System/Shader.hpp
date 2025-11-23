#ifndef _SYSTEM_SHADER_HPP
#define _SYSTEM_SHADER_HPP

#include <string>
#include <System/Graphics/GraphicsHelpers.h>
#include <System/Matrix4x4.hpp>
#include <System/Color.hpp>

namespace System {
    class Shader {
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath){
        
        }

        void use(){
            Graphics::GL::gl_glUseProgram(ID);
        }
        void setBool(const std::string& name, bool value){
            Graphics::GL::gl_glUniform1i(Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string& name, int value){
            Graphics::GL::gl_glUniform1i(Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string& name, float value){
            Graphics::GL::gl_glUniform1f(Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), value);
        }
        void setMat4(const std::string& name, const Matrix4x4& mat){
            Graphics::GL::gl_glUniformMatrix4fv(Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), 1, false, &mat.m00);
        }
        void setColor(const std::string& name, const Color& color){
            Graphics::GL::gl_glUniform4f(Graphics::GL::gl_glGetUniformLocation(ID, name.c_str()), color.r, color.g, color.b, color.a);
        }
    private:
        void checkCompileErrors(unsigned int shader, std::string type){

        }
    };
}

#endif