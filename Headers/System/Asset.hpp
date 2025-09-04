
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
                std::vector<std::tuple<int, int, int>> face_indices;
                std::string face_data;
                while (ss >> face_data) {
                    int v_idx = 0, vt_idx = 0, vn_idx = 0;
                    size_t first_slash = face_data.find('/');
                    size_t second_slash = face_data.find('/', first_slash + 1);

                    v_idx = std::stoi(face_data.substr(0, first_slash));

                    if (first_slash != std::string::npos) { //
                        if (first_slash + 1 != second_slash) {
                            vt_idx = std::stoi(face_data.substr(first_slash + 1, second_slash - first_slash - 1));
                        }
                        if (second_slash != std::string::npos) {
                            vn_idx = std::stoi(face_data.substr(second_slash + 1));
                        }
                    }
                    face_indices.emplace_back(v_idx, vt_idx, vn_idx);
                }

                if (face_indices.size() < 3) {
                    // Not a valid face
                    continue;
                }

                // Triangulation (fan)
                for (size_t i = 1; i < face_indices.size() - 1; ++i) {
                    int indices[3] = { 0, (int)i, (int)i + 1 };

                    for (int j = 0; j < 3; ++j) {
                        auto& face_index = face_indices[indices[j]];
                        int v_idx = std::get<0>(face_index);
                        int vt_idx = std::get<1>(face_index);
                        int vn_idx = std::get<2>(face_index);

                        // Handle negative indices
                        if (v_idx < 0) v_idx = temp_vertices.size() + v_idx + 1;
                        if (vt_idx < 0) vt_idx = temp_uvs.size() + vt_idx + 1;
                        if (vn_idx < 0) vn_idx = temp_normals.size() + vn_idx + 1;

                        if (v_idx > 0) {
                            mesh.vertices.push_back(temp_vertices[v_idx - 1]);
                            mesh.triangles.push_back(mesh.vertices.size() - 1);
                        }
                        if (vt_idx > 0) mesh.uvs.push_back(temp_uvs[vt_idx - 1]);
                        if (vn_idx > 0) mesh.normals.push_back(temp_normals[vn_idx - 1]);
                    }
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