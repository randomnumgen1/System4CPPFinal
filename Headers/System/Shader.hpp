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

        Shader(const char* vertexPath, const char* fragmentPath) {
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try {
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure& e) {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            unsigned int vertex, fragment;
            int success;
            char infoLog[512];

            vertex = System::Graphics::GL::gl_glCreateShader(GL_VERTEX_SHADER);
            System::Graphics::GL::gl_glShaderSource(vertex, 1, &vShaderCode, NULL);
            System::Graphics::GL::gl_glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");

            fragment = System::Graphics::GL::gl_glCreateShader(GL_FRAGMENT_SHADER);
            System::Graphics::GL::gl_glShaderSource(fragment, 1, &fShaderCode, NULL);
            System::Graphics::GL::gl_glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");

            ID = System::Graphics::GL::gl_glCreateProgram();
            System::Graphics::GL::gl_glAttachShader(ID, vertex);
            System::Graphics::GL::gl_glAttachShader(ID, fragment);
            System::Graphics::GL::gl_glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");

            System::Graphics::GL::gl_glDeleteShader(vertex);
            System::Graphics::GL::gl_glDeleteShader(fragment);
        }

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
                System::Graphics::GL::gl_glGetShaderiv(shader, System::Graphics::GL_COMPILE_STATUS, &success);
                if (!success) {
                    System::Graphics::GL::gl_glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
            else {
                System::Graphics::GL::gl_glGetProgramiv(shader, System::Graphics::GL_LINK_STATUS, &success);
                if (!success) {
                    System::Graphics::GL::gl_glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
    };
}

#endif