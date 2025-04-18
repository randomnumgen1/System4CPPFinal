#pragma once
#include <cstdint>
namespace System {
    namespace Drawing {
        struct Size {
            int Width;
            int Height;
            Size(int _width,int _height){
                Width = _width;
                Height = _height;
            }

        };

    }
}