#include <System/Shader.hpp>
#include <System/Graphics/GraphicsHelpers.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace System {
    /*
    Shader::Shader(const char* vertexPath, const char* fragmentPath) {
        std::string vertexCode = ReadFile(vertexPath);
        std::string fragmentCode = ReadFile(fragmentPath);

        unsigned int vertex = CompileShader(vertexCode.c_str(), System::Graphics::shaderTypes::GL_VERTEX_SHADER);
        unsigned int fragment = CompileShader(fragmentCode.c_str(), System::Graphics::shaderTypes::GL_FRAGMENT_SHADER);

        ID = LinkProgram(vertex, fragment);

        System::Graphics::GL::gl_glDeleteShader(vertex);
        System::Graphics::GL::gl_glDeleteShader(fragment);
    }

    Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
        std::string vertexCode = ReadFile(vertexPath);
        std::string fragmentCode = ReadFile(fragmentPath);
        std::string geometryCode = ReadFile(geometryPath);

        unsigned int vertex = CompileShader(vertexCode.c_str(), System::Graphics::shaderTypes::GL_VERTEX_SHADER);
        unsigned int fragment = CompileShader(fragmentCode.c_str(), System::Graphics::shaderTypes::GL_FRAGMENT_SHADER);
        unsigned int geometry = CompileShader(geometryCode.c_str(), System::Graphics::shaderTypes::GL_GEOMETRY_SHADER);

        ID = LinkProgram(vertex, fragment, geometry);

        System::Graphics::GL::gl_glDeleteShader(vertex);
        System::Graphics::GL::gl_glDeleteShader(fragment);
        System::Graphics::GL::gl_glDeleteShader(geometry);
    }
*/
    Shader* Shader::CreateFromSource(const char* vShaderCode, const char* fShaderCode) {
        unsigned int vertex = CompileShader(vShaderCode, System::Graphics::shaderTypes::GL_VERTEX_SHADER);
        unsigned int fragment = CompileShader(fShaderCode, System::Graphics::shaderTypes::GL_FRAGMENT_SHADER);

        Shader* shader = new Shader();
        shader->ID = LinkProgram(vertex, fragment);

        System::Graphics::GL::gl_glDeleteShader(vertex);
        System::Graphics::GL::gl_glDeleteShader(fragment);
        return shader;
    }

    Shader* Shader::CreateFromSource(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode) {
        unsigned int vertex = CompileShader(vShaderCode, System::Graphics::shaderTypes::GL_VERTEX_SHADER);
        unsigned int fragment = CompileShader(fShaderCode, System::Graphics::shaderTypes::GL_FRAGMENT_SHADER);
        unsigned int geometry = CompileShader(gShaderCode, System::Graphics::shaderTypes::GL_GEOMETRY_SHADER);

        Shader* shader = new Shader();
        shader->ID = LinkProgram(vertex, fragment, geometry);

        System::Graphics::GL::gl_glDeleteShader(vertex);
        System::Graphics::GL::gl_glDeleteShader(fragment);
        System::Graphics::GL::gl_glDeleteShader(geometry);
        return shader;
    }

    Shader* Shader::CreateFromStrings(std::string shadername, std::string vertexSource, std::string fragmentSource) {
        Shader* shader = CreateFromSource(vertexSource.c_str(), fragmentSource.c_str());
        if (shader) {
            shader->name = shadername;
        }
        return shader;
    }

    Shader* Shader::CreateFromStrings(std::string shadername, std::string vertexSource, std::string fragmentSource, std::string geometrySource) {
        Shader* shader = CreateFromSource(vertexSource.c_str(), fragmentSource.c_str(), geometrySource.c_str());
        if (shader) {
            shader->name = shadername;
        }
        return shader;
    }

    Shader* Shader::CreateFromFiles(std::string shadername, std::string vertexPath, std::string fragmentPath) {
        try {
            std::string vertexCode = ReadFile(vertexPath.c_str());
            std::string fragmentCode = ReadFile(fragmentPath.c_str());
            return CreateFromStrings(shadername, vertexCode, fragmentCode);
        }
        catch (...) {
            return nullptr;
        }
    }

    Shader* Shader::CreateFromFiles(std::string shadername, std::string vertexPath, std::string fragmentPath, std::string geometryPath) {
        try {
            std::string vertexCode = ReadFile(vertexPath.c_str());
            std::string fragmentCode = ReadFile(fragmentPath.c_str());
            std::string geometryCode = ReadFile(geometryPath.c_str());
            return CreateFromStrings(shadername, vertexCode, fragmentCode, geometryCode);
        }
        catch (...) {
            return nullptr;
        }
    }

    std::string Shader::ReadFile(const char* path) {
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            file.open(path);
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            return stream.str();
        }
        catch (std::ifstream::failure& e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << path << std::endl;
            throw e;
        }
    }

    unsigned int Shader::CompileShader(const char* source, System::Graphics::shaderTypes type) {
        unsigned int shader = System::Graphics::GL::gl_glCreateShader(type);
        System::Graphics::GL::gl_glShaderSource(shader, 1, &source, NULL);
        System::Graphics::GL::gl_glCompileShader(shader);

        // Use a temporary shader object to call checkCompileErrors
        Shader temp;
        std::string typeStr = (type == System::Graphics::shaderTypes::GL_VERTEX_SHADER) ? "VERTEX" :
            (type == System::Graphics::shaderTypes::GL_FRAGMENT_SHADER) ? "FRAGMENT" : "GEOMETRY";
        temp.checkCompileErrors(shader, typeStr);
        return shader;
    }

    unsigned int Shader::LinkProgram(unsigned int vertex, unsigned int fragment, unsigned int geometry) {
        unsigned int program = System::Graphics::GL::gl_glCreateProgram();
        System::Graphics::GL::gl_glAttachShader(program, vertex);
        System::Graphics::GL::gl_glAttachShader(program, fragment);
        if (geometry != 0) {
            System::Graphics::GL::gl_glAttachShader(program, geometry);
        }

        System::Graphics::GL::gl_glBindAttribLocation(program, 0, "aPos");
        System::Graphics::GL::gl_glBindAttribLocation(program, 1, "aNormal");
        System::Graphics::GL::gl_glBindAttribLocation(program, 2, "aUV");

        System::Graphics::GL::gl_glLinkProgram(program);

        Shader temp;
        temp.checkCompileErrors(program, "PROGRAM");
        return program;
    }

}
