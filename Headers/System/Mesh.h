#ifndef _SYSTEM_MESH_H
#define _SYSTEM_MESH_H

#include <System/Vector3.hpp>
#include <vector>
#include <type_traits>
#include <System/Vector2.hpp>
#include <System/Vector4.hpp>
#include <System/Mathf.hpp>
#include <System/Bounds.hpp>





#define ENUM_FLAG_OPERATORS(T) \
    inline T operator~ (T a) { return static_cast<T>(~static_cast<std::underlying_type_t<T>>(a)); } \
    inline T operator| (T a, T b) { return static_cast<T>(static_cast<std::underlying_type_t<T>>(a) | static_cast<std::underlying_type_t<T>>(b)); } \
    inline T operator& (T a, T b) { return static_cast<T>(static_cast<std::underlying_type_t<T>>(a) & static_cast<std::underlying_type_t<T>>(b)); } \
    inline T operator^ (T a, T b) { return static_cast<T>(static_cast<std::underlying_type_t<T>>(a) ^ static_cast<std::underlying_type_t<T>>(b)); } \
    inline T& operator|= (T& a, T b) { a = a | b; return a; } \
    inline T& operator&= (T& a, T b) { a = a & b; return a; } \
    inline T& operator^= (T& a, T b) { a = a ^ b; return a; }
  

enum bitFlags : uint32_t {
            lol,
            NoLongerReadable = 1 << 0,
       
        };
 ENUM_FLAG_OPERATORS(bitFlags);
namespace System{

    class Mesh {
    private:

        bitFlags m_bitFlags;
        
    public:
        std::vector<System::Vector3> vertices;
        std::vector<System::Vector3> normals;
        std::vector<System::Vector2> uvs;
        std::vector<int> triangles;
        System::Bounds bounds;


        bool isReadable() const {
           return (m_bitFlags & bitFlags::NoLongerReadable) != bitFlags::NoLongerReadable;
        }
        void RecalculateBounds() {
            if (vertices.empty()) {
                return; // No vertices, nothing to calculate
            }
            Vector3 minBounds = vertices[0];
            Vector3 maxBounds = vertices[0];
            for (const auto& vertex : vertices) {
                minBounds.x = System::Mathf::Min(minBounds.x, vertex.x);
                minBounds.y = System::Mathf::Min(minBounds.y, vertex.y);
                minBounds.z = System::Mathf::Min(minBounds.z, vertex.z);

                maxBounds.x = System::Mathf::Max(maxBounds.x, vertex.x);
                maxBounds.y = System::Mathf::Max(maxBounds.y, vertex.y);
                maxBounds.z = System::Mathf::Max(maxBounds.z, vertex.z);
            }
            bounds.min = minBounds;
            bounds.max = maxBounds;
            bounds.size = maxBounds - minBounds;
            bounds.center = (maxBounds + minBounds) * 0.5f;
            bounds.extents = bounds.size * 0.5f;
        }
        void RecalculateNormals() {
            normals.clear();
            size_t vertexCount = vertices.size();
            normals.reserve(vertexCount);
            for (size_t i = 0; i < vertexCount; i += 3){
                Vector3 normal = Vector3::Cross(vertices[i + 1] - vertices[i], vertices[i + 2] - vertices[i]).normalized();
                normals.push_back(normal);
                normals.push_back(normal);
                normals.push_back(normal);
            }
        }
        void RecalculateTangents() {
        
        }
        void SetVertices(const std::vector<Vector3>& inVertices) {
            vertices = inVertices;
        }
        void SetVertices(System::Vector3* inVertices,int count) {
            vertices.assign(inVertices, inVertices + count);
        }
        void SetUVs(int channel, System::Vector2* uvs, int count) {

        }
        void SetUVs(int channel, System::Vector3* uvs, int count) {

        }
        void SetUVs(int channel, System::Vector4* uvs, int count) {

        }
        void SetUVs(int channel,const  std::vector<System::Vector2>& new_uvs) {
           
        }
        void SetUVs(int channel, const std::vector<System::Vector3>& new_uvs) {

        }
        void SetUVs(int channel, const std::vector<System::Vector4>& new_uvs) {

        }
        void SetTriangles(std::vector<int> triangles, int submesh, bool calculateBounds = true, int baseVertex = 0) {
        
        }
        void UploadMeshData(bool markNoLongerReadable){
            if (markNoLongerReadable) {
                m_bitFlags = m_bitFlags | bitFlags::NoLongerReadable;
            }
        }


    };
}
#endif