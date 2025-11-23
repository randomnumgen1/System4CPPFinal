#ifndef _SYSTEM_TEXTURE2D_HPP
#define _SYSTEM_TEXTURE2D_HPP


#include <System/Image.hpp>
#include <System/Graphics/GraphicsHelpers.h>
namespace System{
    class Texture2d {
    private:
        System::Image *image;
        bool uploaded = false;
        //textureID
    public:
        Texture2d() {
        //image->m_pixels
        }
        /// <summary>
        /// updates the texture with the current image pixels.
        /// </summary>
        void FullUpdate(){
          //  System::Graphics::GL::glBindTexture(GL_TEXTURE_2D, textureID);
           // System::Graphics::GL::glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, &image->m_pixels[0]);
        }
        /// <summary>
        /// Performs a partial update of a texture region using the specified coordinates and dimensions.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="w"></param>
        /// <param name="h"></param>
        void PartialUpdate(int x, int y, int w, int h){
           // System::Graphics::GL::glBindTexture(GL_TEXTURE_2D, textureID);
           // System::Graphics::GL::glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, (w == 0) ? width : w, (h == 0) ? height : h, format, GL_UNSIGNED_BYTE, texels);
        }
        /// <summary>
        /// Deletes the texture from the GPU.
        /// </summary>
        void Delete() {
          //  System::Graphics::GL::glDeleteTextures(1, &textureID);
        }


    };
}
#endif