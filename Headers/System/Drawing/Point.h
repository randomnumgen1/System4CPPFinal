#pragma once
/*
Point.h
Total Size: 8 Bytes

x - 4 Bytes
y - 4 Bytes
*/
#include <cstdint>
namespace System {
    struct Point {
    public:
        int x;
        int y;
        Point() {
            x = 0;
            y = 0;
        }
        Point(int _x,int _y) {
            x = _x;
            y = _y;
        }


    };

}