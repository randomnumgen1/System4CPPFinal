#pragma once
/*
PointF.h
Total Size: 8 Bytes

x - 4 Bytes
y - 4 Bytes
*/
namespace System {
	struct PointF {
	public:
		float x;
		float y;
		PointF() {
			x = 0.0f;
			y = 0.0f;
		}
		PointF(float _x, float _y) {
			x = _x;
			y = _y;
		}

	};

}