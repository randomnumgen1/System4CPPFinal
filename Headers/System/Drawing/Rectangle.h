#pragma once
#include "Point.h"
namespace System {
    struct Rectangle {
        int x;
        int y;
        int width;
        int height;
        Rectangle() {
            x = 0;
            y = 0;
            width = 0;
            height = 0;
        }
        Rectangle(int _x,int _y,int _width,int _height) {
            x = _x;
            y = _y;
            width = _width;
            height = _height;
        }
        // Check if a point is within this rectangle
        bool Contains(const System::Point& point) const {
            return (point.x >= x - width &&
                    point.x <= x + width &&
                    point.y >= y - height &&
                    point.y <= y + height);
        }
        // Check if this rectangle intersects another
        bool Itersects(const Rectangle& range) const {
            return !(range.x - range.width > x + width ||
                     range.x + range.width < x - width ||
                     range.y - range.height > y + height ||
                     range.y + range.height < y - height);
        }

    };
}