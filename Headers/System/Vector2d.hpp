#ifndef _SYSTEM_VECTOR2D_H
#define _SYSTEM_VECTOR2D_H

namespace System {

	struct Vector2d {
	public:
		union {
			double v[2];
			struct {
				double x;
				double y;
			};
		};
		/*
		 ----------------------------------------------------------------------------------------
		Constructors
		 ----------------------------------------------------------------------------------------
		*/
		Vector2d() {}
		Vector2d(double x, double y) {}









	};
}
#endif