
#ifndef _SYSTEM_ASSET_H
#define _SYSTEM_ASSET_H

#include <string>
#include <System/Mesh.h>
#include <System/Image.hpp>
//each 3d model could have multiple meshes and multiple images (textures)
class Asset {
public:
    std::vector<System::Mesh> meshes;
    std::vector<System::Image> images;


    void LoadOBJ(const std::string& filepath) {

    }
    void LoadDAE(const std::string& filepath) {

    }
    void LoadFBX(const std::string& filepath) {

    }
    void LoadGLB(const std::string& filepath) {

    }
    void LoadGLTF(const std::string& filepath) {

    }
 




};
#endif