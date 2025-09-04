
#ifndef _SYSTEM_ASSET_H
#define _SYSTEM_ASSET_H

#include <string> 

 
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <System/Mesh.h>
#include <System/Image.hpp>
#include <System/Vector2.hpp>
#include <System/Vector3.hpp>

//each 3d model could have multiple meshes and multiple images (textures)
class Asset {
public:
    std::vector<System::Mesh> meshes;
    std::vector<System::Image> images;


    void LoadOBJ(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filepath << std::endl;
            return;
        }

        meshes.emplace_back();
        System::Mesh& mesh = meshes.back();

        std::vector<System::Vector3> temp_vertices;
        std::vector<System::Vector2> temp_uvs;
        std::vector<System::Vector3> temp_normals;

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string keyword;
            ss >> keyword;

            if (keyword == "v") {
                System::Vector3 vertex;
                ss >> vertex.x >> vertex.y >> vertex.z;
                temp_vertices.push_back(vertex);
            }
            else if (keyword == "vt") {
                System::Vector2 uv;
                ss >> uv.x >> uv.y;
                temp_uvs.push_back(uv);
            }
            else if (keyword == "vn") {
                System::Vector3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                temp_normals.push_back(normal);
            }
            else if (keyword == "f") {
                std::string face_data;
                while (ss >> face_data) {
                    std::stringstream face_ss(face_data);
                    std::string index_str;
                    int v_idx = 0, vt_idx = 0, vn_idx = 0;

                    std::getline(face_ss, index_str, '/');
                    if (!index_str.empty()) v_idx = std::stoi(index_str);

                    std::getline(face_ss, index_str, '/');
                    if (!index_str.empty()) vt_idx = std::stoi(index_str);

                    std::getline(face_ss, index_str, '/');
                    if (!index_str.empty()) vn_idx = std::stoi(index_str);

                    if (v_idx > 0) {
                        mesh.vertices.push_back(temp_vertices[v_idx - 1]);
                        mesh.triangles.push_back(mesh.vertices.size() - 1);
                    }
                    if (vt_idx > 0) mesh.uvs.push_back(temp_uvs[vt_idx - 1]);
                    if (vn_idx > 0) mesh.normals.push_back(temp_normals[vn_idx - 1]);
                }
            }
        }
        mesh.RecalculateBounds();
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