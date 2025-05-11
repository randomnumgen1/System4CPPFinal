#include <System/Vector3d.hpp>

System::Vector3d::Vector3d(){
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

System::Vector3d::Vector3d(double new_x, double new_y, double new_z){
    x = new_x;
    y = new_y;
    z = new_z;
}
