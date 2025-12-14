#ifndef _SYSTEM_VECTOR2D_H
#define _SYSTEM_VECTOR2D_H

namespace System {

	struct Vector2d {
	public:
		union {
			double v[2];
			struct {
				double x,y;
			};
		};
		/*
		 ----------------------------------------------------------------------------------------
		Constructors
		 ----------------------------------------------------------------------------------------
		*/
		Vector2d() : x(0.0), y(0.0) {}
		Vector2d(double x, double y) : x(x), y(y) {}









	};
}
#endif