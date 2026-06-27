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
        R32F
    };
    //RenderTextureType
    struct RenderTexture {
        uint32_t renderedTexture;
        uint32_t framebuffer;
        uint32_t depthBuffer;
        int m_width;
        int m_height;


        RenderTexture(int width, int height, int depth)
            : RenderTexture(width, height, depth, RenderTextureType::Default) {}

        RenderTexture(int width, int height, int depth, RenderTextureType RenderType)
            : renderedTexture(0), framebuffer(0), depthBuffer(0), m_width(width), m_height(height) {
            if (RenderType == RenderTextureType::Default) {
                RenderType = RenderTextureType::ARGB32;
            }

            System::Graphics::GL::gl_glGenTextures(1, &renderedTexture);
            System::Graphics::GL::gl_glBindTexture(System::Graphics::GLenum1::GL_TEXTURE_2D, renderedTexture);

            int internalFormat = 0x1908; // GL_RGBA
            unsigned int format = 0x1908; // GL_RGBA
            unsigned int type = 0x1401; // GL_UNSIGNED_BYTE

            if (RenderType == RenderTextureType::R32F) {
                internalFormat = 0x822E; // GL_R32F
                format = 0x1903; // GL_RED
                type = 0x1406; // GL_FLOAT
            }
            else if (RenderType == RenderTextureType::BGRA32) {
                internalFormat = 0x1908;
                format = 0x80E1; // GL_BGRA
            }

            System::Graphics::GL::gl_glTexImage2D((unsigned int)System::Graphics::GLenum1::GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, nullptr);
            System::Graphics::GL::gl_glTexParameteri((unsigned int)System::Graphics::GLenum1::GL_TEXTURE_2D, 0x2801, 0x2601); // GL_TEXTURE_MIN_FILTER, GL_LINEAR
            System::Graphics::GL::gl_glTexParameteri((unsigned int)System::Graphics::GLenum1::GL_TEXTURE_2D, 0x2800, 0x2601); // GL_TEXTURE_MAG_FILTER, GL_LINEAR
            System::Graphics::GL::gl_glTexParameteri((unsigned int)System::Graphics::GLenum1::GL_TEXTURE_2D, 0x2802, 0x812F); // GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE
            System::Graphics::GL::gl_glTexParameteri((unsigned int)System::Graphics::GLenum1::GL_TEXTURE_2D, 0x2803, 0x812F); // GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE

            System::Graphics::GL::gl_glGenFramebuffers(1, &framebuffer);
            System::Graphics::GL::gl_glBindFramebuffer(System::Graphics::GL_FrameBufferTarget::GL_FRAMEBUFFER, framebuffer);
            System::Graphics::GL::gl_glFramebufferTexture2D(System::Graphics::GL_FrameBufferTarget::GL_FRAMEBUFFER, System::Graphics::FramebufferAttachment::ColorAttachment0, System::Graphics::GLenum1::GL_TEXTURE_2D, renderedTexture, 0);

            if (depth > 0) {
                SYSTEM_INTERNAL_glGenRenderbuffers(1, &depthBuffer);
                SYSTEM_INTERNAL_glBindRenderbuffer(0x8D41, depthBuffer); // GL_RENDERBUFFER

                unsigned int depthFormat = 0x81A5; // GL_DEPTH_COMPONENT16
                if (depth > 16) depthFormat = 0x81A6; // GL_DEPTH_COMPONENT24
                if (depth > 24) depthFormat = 0x81A7; // GL_DEPTH_COMPONENT32

                SYSTEM_INTERNAL_glRenderbufferStorage(0x8D41, depthFormat, width, height);
                SYSTEM_INTERNAL_glFramebufferRenderbuffer(0x8D40, 0x8D00, 0x8D41, depthBuffer); // GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER
            }

            System::Graphics::GL::gl_glBindFramebuffer(System::Graphics::GL_FrameBufferTarget::GL_FRAMEBUFFER, 0);
        }

        bool isCreated() const {
            return renderedTexture != 0;
        }
        intptr_t GetNativeTexturePtr() const {
            return  renderedTexture;
        }

        int GetWidth() const {
            return m_width;
        }

        int GetHeight() const {
            return m_height;
        }
        ~RenderTexture() {
            if (renderedTexture != 0) {
                SYSTEM_INTERNAL_glDeleteTextures(1, &renderedTexture);
            }
            if (depthBuffer != 0) {
                SYSTEM_INTERNAL_glDeleteRenderbuffers(1, &depthBuffer);
            }
            if (framebuffer != 0) {
                SYSTEM_INTERNAL_glDeleteFramebuffers(1, &framebuffer);
            }
        }


    };
}
#endif