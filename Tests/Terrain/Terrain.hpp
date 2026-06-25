#ifndef _SYSTEM_TERRAIN_HPP
#define _SYSTEM_TERRAIN_HPP

#include <System/Component.hpp>
#include <System/Vector2.hpp>
#include <System/Mesh.hpp>
#include <System/Shader.hpp>
#include <System/Material.hpp>
#include <string>

namespace System {
    class Terrain : public Component {
    public:
        enum class BrushType {
            Add = 1,
            Subtract = 2,
            Smooth = 3,
            Flatten = 4
        };

        Terrain();
        virtual ~Terrain();

        void Initialize(int width, int height, float size);
		void SaveHeightmapToBitmap(uint32_t tex, int width, int height, const std::string& filename)
        void ModifyHeight(Vector2 uv, float radius, float strength, BrushType type, float targetHeight = 0.0f);

        uint32_t GetHeightmapTexture() const { return m_texHeightA; }
        Mesh* GetMesh() const { return m_mesh; }
        
        float GetSize() const { return m_size; }
        int GetResolutionW() const { return m_mapWidth; }
        int GetResolutionH() const { return m_mapHeight; }

        void Update() override;

        Component* Clone() const override {
            return new Terrain(); // Basic clone
        }

    private:
        int m_mapWidth;
        int m_mapHeight;
        float m_size;

        uint32_t m_texHeightA;
        uint32_t m_texHeightB;
        uint32_t m_fboA;
        uint32_t m_fboB;

        Shader* m_brushShader;
        Mesh* m_mesh;
        Mesh* m_quadMesh;

        void InitHeightmap();
        void InitShaders();
        void InitMesh();
    };
}

#endif
