#ifndef _SYSTEM_VECTOR3D_H
#define _SYSTEM_VECTOR3D_H

namespace System{

	struct Vector3d {
		public:
			union {
				double v[3];
				struct {
					double x;
					double y;
					double z;
				};
			};
			/*
			 ----------------------------------------------------------------------------------------
			Constructors
			 ----------------------------------------------------------------------------------------
			*/		
			Vector3d();
			Vector3d(double new_x, double new_y, double new_z);
		

		
	};
}
#endif