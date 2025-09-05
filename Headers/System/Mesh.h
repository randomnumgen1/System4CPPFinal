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
#include <variant>
#include <string>

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
            Modified = 1 << 1
       
        };
 ENUM_FLAG_OPERATORS(bitFlags);
namespace System{

    class Mesh {
    private:

        bitFlags m_bitFlags;
        
    public:
        enum MeshTopology {
            Triangles,
            Quads,
            Lines,
            LineStrip,
            Points
        };
        struct SubMeshDescriptor {
            int indexStart;
            int indexCount;
            int baseVertex;
            MeshTopology topology;
        };



        std::vector<System::Vector3> vertices;
        std::vector<System::Vector3> normals; 
        std::vector<int> indices;
        System::Bounds bounds;
        std::vector<SubMeshDescriptor> submeshes;
        std::vector<Vector4> tangents;

        std::variant<std::monostate,std::vector<Vector2>, std::vector<Vector3>, std::vector<Vector4>> uvs[8];



         

        void GetNativeIndexBufferPtr() {
        
        }
        void GetNativeVertexBufferPtr() {
        
        }
  

        void GetUVs(int channel, std::vector<Vector2>& outUVs) {
            if ((channel < 0) || (channel > 7)) throw std::out_of_range("Channel must be between 0 and 7");
            if (auto* pval = std::get_if<std::vector<Vector2>>(&uvs[channel])) {
                outUVs = *pval;
            }
            else {
                throw std::runtime_error("UV channel " + std::to_string(channel) + " does not contain Vector2 data.");
            }
        }

        void GetUVs(int channel, std::vector<Vector3>& outUVs) {
            if ((channel < 0) || (channel > 7)) throw std::out_of_range("Channel must be between 0 and 7");
            if (auto* pval = std::get_if<std::vector<Vector3>>(&uvs[channel])) {
                outUVs = *pval;
            }
            else {
                throw std::runtime_error("UV channel " + std::to_string(channel) + " does not contain Vector3 data.");
            }
        }

        void GetUVs(int channel, std::vector<Vector4>& outUVs) {
            if ((channel < 0) || (channel > 7)) throw std::out_of_range("Channel must be between 0 and 7");
            if (auto* pval = std::get_if<std::vector<Vector4>>(&uvs[channel])) {
                outUVs = *pval;
            }else{
                throw std::runtime_error("UV channel " + std::to_string(channel) + " does not contain Vector4 data.");
            }
        }



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
            for (size_t i = 0; i < indices.size(); i += 3) {
                // Get the indices of the vertices that form the triangle
                int idx0 = indices[i];
                int idx1 = indices[i + 1];
                int idx2 = indices[i + 2];

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
            if (vertices.empty() || normals.empty()) return;

            std::vector<Vector3> tan1(vertices.size(), Vector3(0, 0, 0));
            std::vector<Vector3> tan2(vertices.size(), Vector3(0, 0, 0));
            tangents.resize(vertices.size(), Vector4(0, 0, 0, 0));

            std::vector<Vector2> uv0;
            GetUVs(0, uv0);

            for (size_t i = 0; i + 2 < indices.size(); i += 3) {
                int i0 = indices[i];
                int i1 = indices[i + 1];
                int i2 = indices[i + 2];

                const Vector3& v0 = vertices[i0];
                const Vector3& v1 = vertices[i1];
                const Vector3& v2 = vertices[i2];

                const Vector2& uv0_0 = uv0[i0];
                const Vector2& uv0_1 = uv0[i1];
                const Vector2& uv0_2 = uv0[i2];

                Vector3 edge1 = v1 - v0;
                Vector3 edge2 = v2 - v0;

                Vector2 deltaUV1 = uv0_1 - uv0_0;
                Vector2 deltaUV2 = uv0_2 - uv0_0;

                float r = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;
                if (r == 0.0f) r = 1.0f;

                float invR = 1.0f / r;
                Vector3 tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * invR;
                Vector3 bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * invR;

                tan1[i0] += tangent;
                tan1[i1] += tangent;
                tan1[i2] += tangent;

                tan2[i0] += bitangent;
                tan2[i1] += bitangent;
                tan2[i2] += bitangent;
            }

            for (size_t i = 0; i < vertices.size(); ++i) {
                const Vector3& n = normals[i];
                const Vector3& t = tan1[i];

                Vector3 tangent = (t - n * Vector3::Dot(n, t)).normalized();
                float w = (Vector3::Dot(Vector3::Cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;
                tangents[i] = Vector4(tangent.x, tangent.y, tangent.z, w);
            }
        }
        void SetVertices(const std::vector<Vector3>& inVertices) {
            vertices = inVertices;
            m_bitFlags |= bitFlags::Modified;
        }
        void SetVertices(System::Vector3* inVertices,int count) {
            vertices.assign(inVertices, inVertices + count);
            m_bitFlags |= bitFlags::Modified;
        }
        void SetUVs(int channel, Vector2* data, int count) {
            if (channel < 0 || channel > 7) throw std::out_of_range("Channel must be between 0 and 7");
            uvs[channel] = std::vector<Vector2>(data, data + count);
            m_bitFlags |= bitFlags::Modified;
        }

        void SetUVs(int channel, Vector3* data, int count) {
            if (channel < 0 || channel > 7) throw std::out_of_range("Channel must be between 0 and 7");
            uvs[channel] = std::vector<Vector3>(data, data + count);
            m_bitFlags |= bitFlags::Modified;
        }

        void SetUVs(int channel, Vector4* data, int count) {
            if (channel < 0 || channel > 7) throw std::out_of_range("Channel must be between 0 and 7");
            uvs[channel] = std::vector<Vector4>(data, data + count);
            m_bitFlags |= bitFlags::Modified;
        }
        void SetUVs(int channel,const  std::vector<System::Vector2>& new_uvs) {
            if ((channel < 0) || (channel > 7)) [[unlikely]] {
                throw std::out_of_range("Channel must be between 0 and 7");
            }
            uvs[channel] = new_uvs;
            m_bitFlags |= bitFlags::Modified;
        }
        void SetUVs(int channel, const std::vector<System::Vector3>& new_uvs) {
            if ((channel < 0) || (channel > 7)) [[unlikely]] {
                throw std::out_of_range("Channel must be between 0 and 7");
            }
            uvs[channel] = new_uvs;
            m_bitFlags |= bitFlags::Modified;
        }
        void SetUVs(int channel, const std::vector<System::Vector4>& new_uvs) {
            if ((channel < 0) || (channel > 7)) [[unlikely]] {
                throw std::out_of_range("Channel must be between 0 and 7");
            }
            uvs[channel] = new_uvs;
            m_bitFlags |= bitFlags::Modified;

        }
        void SetTriangles(const std::vector<int>& triangles, int submesh, bool calculateBounds = true, int baseVertex = 0) {
            m_bitFlags |= bitFlags::Modified;
            if (submesh < 0) {
                throw std::out_of_range("Submesh index is out of range.");
            }

            if (submeshes.size() <= submesh) {
                submeshes.resize(submesh + 1);
            }

            submeshes[submesh] = { (int)indices.size(), (int)triangles.size(), baseVertex, MeshTopology::Triangles };
            indices.insert(indices.end(), triangles.begin(), triangles.end());

            if (calculateBounds) {
                RecalculateBounds();
            }
        }
        const SubMeshDescriptor& GetSubMesh(int index) const {
            if (index < 0 || index >= submeshes.size()) throw std::out_of_range("Submesh index is out of range.");
            return submeshes[index];
        }
        std::vector<int> GetIndices(int submesh) const {
            if (submesh < 0 || submesh >= submeshes.size()) {
                throw std::out_of_range("Submesh index is out of range.");
            }

            const auto& desc = submeshes[submesh];
            return std::vector<int>(indices.begin() + desc.indexStart, indices.begin() + desc.indexStart + desc.indexCount);
        }
        void Clear(bool keepVertexLayout = true) {
            vertices.clear();
            normals.clear();
            tangents.clear();
            indices.clear();
            submeshes.clear();
            bounds = Bounds(); 

            if (!keepVertexLayout) {
                for (int i = 0; i < 8; ++i) {
                    uvs[i] = std::monostate{}; // or leave untouched if you want to preserve type
                }
            }
        }
        void SetSubMesh(int index, const SubMeshDescriptor& desc) {
            if (index < 0) {
                throw std::out_of_range("Submesh index is out of range.");
            }
            if (submeshes.size() <= index) {
                submeshes.resize(index + 1);
            }
            submeshes[index] = desc;
        }

        int getSubMeshCount() const {
            return submeshes.size();
        }

        void setSubMeshCount(int count) {
            submeshes.resize(count);
        }
        void UploadMeshData(bool markNoLongerReadable) {
            if ((m_bitFlags & bitFlags::Modified) == 0) return;

            if ((m_bitFlags & bitFlags::NoLongerReadable) != 0) {
                throw std::out_of_range("Mesh was deleted from CPU side and is no longer readable.");
            }

            // GPU upload here...

            if (markNoLongerReadable) {
                m_bitFlags |= bitFlags::NoLongerReadable;
            }
            //clear the modified flag
            m_bitFlags &= ~bitFlags::Modified;
        } 


    };
}
#endif