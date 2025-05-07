#ifndef _SYSTEM_RENDERTEXTURE_H
#define _SYSTEM_RENDERTEXTURE_H

#include <stdint.h>
#include <System/Graphics/GraphicsHelpers.h>
typedef int* IntPtr_t;
namespace System {
    struct RenderTexture {
        GLuint renderedTexture;
        RenderTexture() {
           
            System::Graphics::GL::gl_glGenTextures(1, &renderedTexture);
            System::Graphics::GL::gl_glBindTexture(GL_TEXTURE_2D, renderedTexture);
            System::Graphics::GL::gl_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
            System::Graphics::GL::gl_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            System::Graphics::GL::gl_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        }
        bool isCreated() const {
            return renderedTexture != 0;
        }
        intptr_t GetNativeTexturePtr() const{
            return  renderedTexture ;
        }

        int GetWidth() {}
        int GetHeight() {}
        ~RenderTexture() {
            System::Graphics::GL::gl_glDeleteTextures(1, &renderedTexture);
        }

    
    };
}
#endif