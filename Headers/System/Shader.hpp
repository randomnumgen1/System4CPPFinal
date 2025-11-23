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
        
        }
        void setBool(const std::string& name, bool value){
        }
        void setInt(const std::string& name, int value){
        }
        void setFloat(const std::string& name, float value){
        }
        void setMat4(const std::string& name, const Matrix4x4& mat){
        }
        void setColor(const std::string& name, const Color& color){
        }
    private:
        void checkCompileErrors(unsigned int shader, std::string type){
        }
    };
}

#endif