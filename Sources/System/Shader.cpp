#include <System/Shader.hpp>
#include <System/Graphics/GraphicsHelpers.h>

namespace System {

    Shader::Shader(const char* vertexPath, const char* fragmentPath) {
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
        vertex = System::Graphics::GL::gl_glCreateShader(System::Graphics::shaderTypes::GL_VERTEX_SHADER);
        System::Graphics::GL::gl_glShaderSource(vertex, 1, &vShaderCode, NULL);
        System::Graphics::GL::gl_glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        fragment = System::Graphics::GL::gl_glCreateShader(System::Graphics::shaderTypes::GL_FRAGMENT_SHADER);
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





}