
#ifndef _SYSTEM_ASSET_H
#define _SYSTEM_ASSET_H

#include <string> 

 
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <tuple>
#include <map>
#include <System/Mesh.h>
#include <System/Image.hpp>
#include <System/Vector2.hpp>
#include <System/Vector3.hpp>

//each 3d model could have multiple meshes and multiple images (textures)
class Asset {
public:
    std::vector<System::Mesh> meshes;
    std::vector<System::Image> images;

    // Group-Aware Mesh Segmentation - OBJ files often use g (group) alongside o. You could treat each g as a submesh or segment within an object:
    void LoadOBJ(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filepath << std::endl;
            return;
        }

        System::Mesh* currentMesh = nullptr;
        std::map<std::tuple<int, int, int>, int> vertex_map;

        std::vector<System::Vector3> temp_vertices;
        std::vector<System::Vector2> temp_uvs;
        std::vector<System::Vector3> temp_normals;

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string keyword;
            ss >> keyword;

            if (keyword == "o" || keyword == "g") {
                if (currentMesh != nullptr) {
                    vertex_map.clear();
                }
                meshes.emplace_back();
                currentMesh = &meshes.back();
            }
            else if (keyword == "v") {
                System::Vector3 vertex;
                ss >> vertex.x >> vertex.y >> vertex.z;
                temp_vertices.push_back(vertex);
            }
            else if (keyword == "usemtl") {
                std::string materialName;
                ss >> materialName;

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
                if (currentMesh == nullptr) {
                    meshes.emplace_back();
                    currentMesh = &meshes.back();
                }

                std::vector<int> face_vertex_indices;
                std::string face_data;
                while (ss >> face_data) {
                    int v_idx = 0, vt_idx = 0, vn_idx = 0;

                    std::stringstream face_ss(face_data);
                    std::string segment;
                    std::vector<std::string> segments;
                    while (std::getline(face_ss, segment, '/')) {
                        segments.push_back(segment);
                    }

                    v_idx = segments.size() > 0 && !segments[0].empty() ? std::stoi(segments[0]) : 0;
                    vt_idx = segments.size() > 1 && !segments[1].empty() ? std::stoi(segments[1]) : 0;
                    vn_idx = segments.size() > 2 && !segments[2].empty() ? std::stoi(segments[2]) : 0;

                    std::tuple<int, int, int> face_index_tuple(v_idx, vt_idx, vn_idx);

                    auto it = vertex_map.find(face_index_tuple);
                    if (it != vertex_map.end()) {
                        face_vertex_indices.push_back(it->second);
                    }
                    else {
                        // Handle negative indices
                        if (v_idx < 0) v_idx = temp_vertices.size() + v_idx + 1;
                        if (vt_idx < 0) vt_idx = temp_uvs.size() + vt_idx + 1;
                        if (vn_idx < 0) vn_idx = temp_normals.size() + vn_idx + 1;

                        if (v_idx > 0 && (size_t)v_idx <= temp_vertices.size()) {
                            currentMesh->vertices.push_back(temp_vertices[v_idx - 1]);
                        }
                        else {
                            currentMesh->vertices.push_back(System::Vector3(0, 0, 0));
                        }

                        if (vt_idx > 0 && (size_t)vt_idx <= temp_uvs.size()) {
                            currentMesh->uvs.push_back(temp_uvs[vt_idx - 1]);
                        }
                        else {
                            currentMesh->uvs.push_back(System::Vector2(0, 0));
                        }

                        if (vn_idx > 0 && (size_t)vn_idx <= temp_normals.size()) {
                            currentMesh->normals.push_back(temp_normals[vn_idx - 1]);
                        }
                        else {
                            currentMesh->normals.push_back(System::Vector3(0, 0, 0));
                        }

                        int new_vertex_index = currentMesh->vertices.size() - 1;
                        vertex_map[face_index_tuple] = new_vertex_index;
                        face_vertex_indices.push_back(new_vertex_index);
                    }
                }

                if (face_vertex_indices.size() < 3) {
                    // Not a valid face
                    continue;
                }

                // Triangulation (fan)
                for (size_t i = 1; i < face_vertex_indices.size() - 1; ++i) {
                    currentMesh->triangles.push_back(face_vertex_indices[0]);
                    currentMesh->triangles.push_back(face_vertex_indices[i]);
                    currentMesh->triangles.push_back(face_vertex_indices[i + 1]);
                }
            }
        }
        for (auto& mesh : meshes) {
            mesh.RecalculateBounds();
        }
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