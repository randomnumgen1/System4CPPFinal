#ifndef _SYSTEM_RENDERTEXTURE_H
#define _SYSTEM_RENDERTEXTURE_H

#include <stdint.h>
#include <System/Graphics/GraphicsHelpers.h>
typedef int* IntPtr_t;
namespace System {
    enum class RenderTextureType : uint32_t {
        Default = 0,
        /// <summary>
        /// 8 bits per channel
        /// </summary>
        ARGB32,
        BGRA32,
       // 8 - bit red channel (good for grayscale)
        R8
    };
    //RenderTextureType
    struct RenderTexture {
        uint32_t renderedTexture;
        int m_width;
        int m_height;
        
    
        RenderTexture(int width, int height, int depth)
            : RenderTexture(width, height, depth, RenderTextureType::Default) {}

        RenderTexture(int width, int height, int depth, RenderTextureType RenderType)
            : renderedTexture(0), m_width(width), m_height(height) {
            if (RenderType == RenderTextureType::Default) {
                throw std::runtime_error("RenderTexture: default not supported yet");
            }
            System::Graphics::GL::gl_glGenTextures(1, &renderedTexture);
            System::Graphics::GL::gl_glBindTexture(System::Graphics::GLenum1::GL_TEXTURE_2D, renderedTexture);
            if (RenderType == RenderTextureType::R8) {
                // Red channel only, 8 bits

                /*
                System::Graphics::GL::gl_glTexImage2D(
                    GL_TEXTURE_2D,  // Target
                    0, // Mipmap level 0
                    GL_R8,  // Internal GPU storage format
                    width, // Width
                    height,  // Height
                    0,  // Border (must be 0)
                    GL_RED,  // Source data format
                    GL_UNSIGNED_BYTE, // Source data data type
                    nullptr// Pointer to raw CPU data
                ); */
            }else{
                throw std::runtime_error("RenderTexture: type not supported yet");
            }
            System::Graphics::GL::gl_glBindTexture(System::Graphics::GLenum1::GL_TEXTURE_2D, 0);
           
        }
        bool isCreated() const {
            return renderedTexture != 0;
        }
        intptr_t GetNativeTexturePtr() const{
            return  renderedTexture ;
        }

        int GetWidth() const {
            return m_width;
        }

        int GetHeight() const {
            return m_height;
        }
        ~RenderTexture() {
            if (renderedTexture != 0) {
               // System::Graphics::GL::gl_glDeleteTextures(1, &renderedTexture);
            } 
        }

    
    };
}
#endif