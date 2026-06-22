#ifndef _SYSTEM_RENDERTEXTURE_H
#define _SYSTEM_RENDERTEXTURE_H

#include <stdint.h>
#include <System/Graphics/GraphicsHelpers.h>
typedef int* IntPtr_t;
namespace System {
    struct RenderTexture {
        uint32_t renderedTexture;
        
        RenderTexture(int width, int height, int depth){
            renderedTexture = 0;
            System::Graphics::GL::gl_glGenTextures(1, &renderedTexture);
            System::Graphics::GL::gl_glBindTexture(System::Graphics::GLenum1::GL_TEXTURE_2D, renderedTexture);
           // System::Graphics::GL::gl_glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);


        }
        bool isCreated() const {
            return renderedTexture != 0;
        }
        intptr_t GetNativeTexturePtr() const{
            return  renderedTexture ;
        }

        int GetWidth() {
            return 0;
        }
        int GetHeight() {
            return 0;
        }
        ~RenderTexture() {
         //   System::Graphics::GL::gl_glDeleteTextures(1, &renderedTexture);
        }

    
    };
}
#endif