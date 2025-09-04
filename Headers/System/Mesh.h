#ifndef _SYSTEM_MESH_H
#define _SYSTEM_MESH_H

#include <System/Vector3.hpp>
#include <vector>
#include <type_traits>
#include <System/Vector2.hpp>
#include <System/Vector4.hpp>
#include <System/Mathf.hpp>
#include <System/Bounds.hpp>
#include <stdexcept>





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
        int subMeshCount;

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
            // Clear existing normals and resize to the same size as vertices, initialized to zero.
            normals.assign(vertices.size(), Vector3(0, 0, 0));

            // Iterate over each triangle
            for (size_t i = 0; i < triangles.size(); i += 3) {
                // Get the indices of the vertices that form the triangle
                int idx0 = triangles[i];
                int idx1 = triangles[i + 1];
                int idx2 = triangles[i + 2];

                // Get the vertices of the triangle
                const Vector3& v0 = vertices[idx0];
                const Vector3& v1 = vertices[idx1];
                const Vector3& v2 = vertices[idx2];

                // Calculate the normal of the triangle
                Vector3 normal = Vector3::Cross(v1 - v0, v2 - v0);

                // Add the normal to the normals of the three vertices
                normals[idx0] += normal;
                normals[idx1] += normal;
                normals[idx2] += normal;
            }

            // Normalize all the normals
            for (auto& normal : normals) {
                normal.Normalize();
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
            if ((channel < 0) || (channel > 7)) [[unlikely]] {
                throw std::out_of_range("Channel must be between 0 and 7");
            }
        }
        void SetUVs(int channel, System::Vector3* uvs, int count) {
            if ((channel < 0) || (channel > 7)) [[unlikely]] {
                throw std::out_of_range("Channel must be between 0 and 7");
            }
        }
        void SetUVs(int channel, System::Vector4* uvs, int count) {
            if ((channel < 0) || (channel > 7)) [[unlikely]] {
                throw std::out_of_range("Channel must be between 0 and 7");
            }
        }
        void SetUVs(int channel,const  std::vector<System::Vector2>& new_uvs) {
            if ((channel < 0) || (channel > 7)) [[unlikely]] {
                throw std::out_of_range("Channel must be between 0 and 7");
            }
        }
        void SetUVs(int channel, const std::vector<System::Vector3>& new_uvs) {
            if ((channel < 0) || (channel > 7)) [[unlikely]] {
                throw std::out_of_range("Channel must be between 0 and 7");
            }
        }
        void SetUVs(int channel, const std::vector<System::Vector4>& new_uvs) {
            if ((channel < 0) || (channel > 7)) [[unlikely]] {
                throw std::out_of_range("Channel must be between 0 and 7");
            }
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