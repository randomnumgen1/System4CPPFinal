#pragma once
#include "Pointf.h"
namespace System {
    struct RectangleF {
        float x;
        float y;
        float width;
        float height;
        RectangleF() {
            x = 0.0f;
            y = 0.0f;
            width = 0.0f;
            height = 0.0f;
        }
        RectangleF(float _x, float _y, float _width, float _height) {
            x = _x;
            y = _y;
            width = _width;
            height = _height;
        }
        // Check if a point is within this rectangle
        bool Contains(const System::PointF& point) const {
            return (point.x >= x - width &&
                    point.x <= x + width &&
                    point.y >= y - height &&
                    point.y <= y + height);
        }
        // Check if this rectangle intersects another
        bool Intersects(const RectangleF& range) const {
            return !(range.x - range.width > x + width ||
                     range.x + range.width < x - width ||
                     range.y - range.height > y + height ||
                     range.y + range.height < y - height);
        }
    };
}