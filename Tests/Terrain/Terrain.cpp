#include <System/Components/Terrain.hpp>
#include <System/Graphics/GraphicsHelpers.h>
#include <System/Time.hpp>
#include <System/MeshFilter.hpp>
#include <System/MeshRenderer.hpp>
#include <System/GameObject.hpp>
#include <vector>
#include <algorithm>

namespace System {

const char* terrainBrushVertexShader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;
out vec2 UV;
void main() {
    gl_Position = vec4(aPos.x * 2.0, aPos.y * 2.0, 0.0, 1.0);
    UV = aUV;
}
)";

const char* terrainBrushFragmentShader = R"(
#version 330 core
in vec2 UV;
out float FragColor;
uniform sampler2D u_SrcHeightmap;
uniform vec2 u_BrushUV;
uniform float u_Radius;
uniform float u_Strength;
uniform int u_BrushType;
uniform float u_TargetHeight;

void main() {
    float height = texture(u_SrcHeightmap, UV).r;
    float dist = distance(UV, u_BrushUV);
    float influence = smoothstep(u_Radius, 0.0, dist);
    
    if (u_BrushType == 1) { // Add
        height += influence * u_Strength;
    } else if (u_BrushType == 2) { // Subtract
        height -= influence * u_Strength;
    } else if (u_BrushType == 3) { // Smooth
        // Very basic box blur for smoothing
        vec2 texelSize = 1.0 / textureSize(u_SrcHeightmap, 0);
        float avg = 0.0;
        for(int x = -1; x <= 1; x++) {
            for(int y = -1; y <= 1; y++) {
                avg += texture(u_SrcHeightmap, UV + vec2(x, y) * texelSize).r;
            }
        }
        avg /= 9.0;
        height = mix(height, avg, influence * u_Strength);
    } else if (u_BrushType == 4) { // Flatten
        height = mix(height, u_TargetHeight, influence * u_Strength);
    }
    
    FragColor = height;
}
)";

Terrain::Terrain() : m_mapWidth(512), m_mapHeight(512), m_size(100.0f), 
                     m_texHeightA(0), m_texHeightB(0), m_fboA(0), m_fboB(0),
                     m_brushShader(nullptr), m_mesh(nullptr), m_quadMesh(nullptr) {
}

Terrain::~Terrain() {
    using namespace Graphics;
    if (m_texHeightA) GL::gl_glDeleteTextures(1, &m_texHeightA);
    if (m_texHeightB) GL::gl_glDeleteTextures(1, &m_texHeightB);
    if (m_fboA) GL::gl_glDeleteFramebuffers(1, &m_fboA);
    if (m_fboB) GL::gl_glDeleteFramebuffers(1, &m_fboB);
    if (m_brushShader) delete m_brushShader;
    if (m_mesh) delete m_mesh;
    if (m_quadMesh) delete m_quadMesh;
}

void Terrain::Initialize(int width, int height, float size) {
    m_mapWidth = width;
    m_mapHeight = height;
    m_size = size;

    InitHeightmap();
    InitShaders();
    InitMesh();

    // Setup MeshFilter and MeshRenderer if they don't exist
    MeshFilter* mf = gameObject()->GetComponent<MeshFilter>();
    if (!mf) mf = gameObject()->AddComponent<MeshFilter>();
    mf->mesh = m_mesh;

    MeshRenderer* mr = gameObject()->GetComponent<MeshRenderer>();
    if (!mr) mr = gameObject()->AddComponent<MeshRenderer>();
}
void Terrain::SaveHeightmapToBitmap(uint32_t tex, int width, int height, const std::string& filename) {
    std::vector<float> data(width * height);
    uint32_t tempFbo;
    GL::gl_glGenFramebuffers(1, &tempFbo);
    GL::gl_glBindFramebuffer(GL_FrameBufferTarget::GL_FRAMEBUFFER, tempFbo);
    GL::gl_glFramebufferTexture2D(GL_FrameBufferTarget::GL_FRAMEBUFFER, GL_FramebufferAttachment::COLOR_ATTACHMENT0, GLenum1::GL_TEXTURE_2D, tex, 0);
    GL::gl_glReadPixels(0, 0, width, height, PixelFormat::RED, PixelType::FLOAT, data.data());
    
    GL::gl_glBindFramebuffer(GL_FrameBufferTarget::GL_FRAMEBUFFER, 0);
    GL::gl_glDeleteFramebuffers(1, &tempFbo);

    std::cout << "Saving heightmap to " << filename << "..." << std::endl;
    Image img(width, height);
    for (int i = 0; i < width * height; ++i) {
        float h = data[i];
        uint8_t val = (uint8_t)(std::max(0.0f, std::min(1.0f, h)) * 255.0f);
        img.m_pixels[i * 4 + 0] = val; // R
        img.m_pixels[i * 4 + 1] = val; // G
        img.m_pixels[i * 4 + 2] = val; // B
        img.m_pixels[i * 4 + 3] = 255; // A
    }
    img.Save(filename, Image::BMP);
    // Cleanup temp FBO
}
void Terrain::InitHeightmap() {
    using namespace Graphics;
    GL::gl_glGenTextures(1, &m_texHeightA);
    GL::gl_glBindTexture(GLenum1::GL_TEXTURE_2D, m_texHeightA);
    GL::gl_glTexImage2D(GLenum1::GL_TEXTURE_2D, 0, PixelFormat::R32F, m_mapWidth, m_mapHeight, 0, PixelFormat::RED, PixelType::FLOAT, nullptr);
    GL::gl_glTexParameteri(GLenum1::GL_TEXTURE_2D, TextureParamName::MinFilter, TextureParam::Linear);
    GL::gl_glTexParameteri(GLenum1::GL_TEXTURE_2D, TextureParamName::MagFilter, TextureParam::Linear);
    GL::gl_glTexParameteri(GLenum1::GL_TEXTURE_2D, TextureParamName::WrapS, TextureParam::ClampToEdge);
    GL::gl_glTexParameteri(GLenum1::GL_TEXTURE_2D, TextureParamName::WrapT, TextureParam::ClampToEdge);

    GL::gl_glGenFramebuffers(1, &m_fboA);
    GL::gl_glBindFramebuffer(GL_FrameBufferTarget::GL_FRAMEBUFFER, m_fboA);
    GL::gl_glFramebufferTexture2D(GL_FrameBufferTarget::GL_FRAMEBUFFER, GL_FramebufferAttachment::COLOR_ATTACHMENT0, GLenum1::GL_TEXTURE_2D, m_texHeightA, 0);

    GL::gl_glGenTextures(1, &m_texHeightB);
    GL::gl_glBindTexture(GLenum1::GL_TEXTURE_2D, m_texHeightB);
    GL::gl_glTexImage2D(GLenum1::GL_TEXTURE_2D, 0, PixelFormat::R32F, m_mapWidth, m_mapHeight, 0, PixelFormat::RED, PixelType::FLOAT, nullptr);
    GL::gl_glTexParameteri(GLenum1::GL_TEXTURE_2D, TextureParamName::MinFilter, TextureParam::Linear);
    GL::gl_glTexParameteri(GLenum1::GL_TEXTURE_2D, TextureParamName::MagFilter, TextureParam::Linear);
    GL::gl_glTexParameteri(GLenum1::GL_TEXTURE_2D, TextureParamName::WrapS, TextureParam::ClampToEdge);
    GL::gl_glTexParameteri(GLenum1::GL_TEXTURE_2D, TextureParamName::WrapT, TextureParam::ClampToEdge);

    GL::gl_glGenFramebuffers(1, &m_fboB);
    GL::gl_glBindFramebuffer(GL_FrameBufferTarget::GL_FRAMEBUFFER, m_fboB);
    GL::gl_glFramebufferTexture2D(GL_FrameBufferTarget::GL_FRAMEBUFFER, GL_FramebufferAttachment::COLOR_ATTACHMENT0, GLenum1::GL_TEXTURE_2D, m_texHeightB, 0);

    // Clear to zero
    GL::gl_glBindFramebuffer(GL_FrameBufferTarget::GL_FRAMEBUFFER, m_fboA);
    GL::gl_glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    GL::gl_glClear(GL_BitField::COLOR_BUFFER_BIT);
    GL::gl_glBindFramebuffer(GL_FrameBufferTarget::GL_FRAMEBUFFER, m_fboB);
    GL::gl_glClear(GL_BitField::COLOR_BUFFER_BIT);
    GL::gl_glBindFramebuffer(GL_FrameBufferTarget::GL_FRAMEBUFFER, 0);
}

void Terrain::InitShaders() {
    m_brushShader = Shader::CreateFromSource(terrainBrushVertexShader, terrainBrushFragmentShader);
}

void Terrain::InitMesh() {
    m_mesh = new Mesh();
    std::vector<Vector3> vertices;
    std::vector<Vector2> uvs;
    std::vector<int> indices;

    int resX = 100;
    int resY = 100;

    float stepX = 1.0f / (resX - 1);
    float stepY = 1.0f / (resY - 1);

    for (int j = 0; j < resY; ++j) {
        for (int i = 0; i < resX; ++i) {
            vertices.push_back(Vector3((i * stepX - 0.5f) * m_size, 0.0f, (j * stepY - 0.5f) * m_size));
            uvs.push_back(Vector2(i * stepX, j * stepY));
        }
    }

    for (int j = 0; j < resY - 1; ++j) {
        for (int i = 0; i < resX - 1; ++i) {
            int row1 = j * resX;
            int row2 = (j + 1) * resX;
            indices.push_back(row1 + i);
            indices.push_back(row1 + i + 1);
            indices.push_back(row2 + i);

            indices.push_back(row1 + i + 1);
            indices.push_back(row2 + i + 1);
            indices.push_back(row2 + i);
        }
    }

    m_mesh->SetVertices(vertices);
    m_mesh->SetUVs(0, uvs);
    m_mesh->SetTriangles(indices, 0);
    m_mesh->RecalculateNormals();
    m_mesh->UploadMeshData(false);

    m_quadMesh = new Mesh();
    m_quadMesh->SetVertices({Vector3(-0.5f, -0.5f, 0.0f), Vector3(0.5f, -0.5f, 0.0f), Vector3(0.5f, 0.5f, 0.0f), Vector3(-0.5f, 0.5f, 0.0f)});
    m_quadMesh->SetUVs(0, {Vector2(0, 0), Vector2(1, 0), Vector2(1, 1), Vector2(0, 1)});
    m_quadMesh->SetTriangles({0, 1, 2, 0, 2, 3}, 0);
    m_quadMesh->UploadMeshData(false);
}

void Terrain::ModifyHeight(Vector2 uv, float radius, float strength, BrushType type, float targetHeight) {
    using namespace Graphics;
    GL::gl_glViewport(0, 0, m_mapWidth, m_mapHeight);
    GL::gl_glBindFramebuffer(GL_FrameBufferTarget::GL_FRAMEBUFFER, m_fboB);
    m_brushShader->use();
    
    GL::gl_glActiveTexture(TextureUnit::Texture0);
    GL::gl_glBindTexture(GLenum1::GL_TEXTURE_2D, m_texHeightA);
    m_brushShader->setInt("u_SrcHeightmap", 0);
    GL::gl_glUniform2f(GL::gl_glGetUniformLocation(m_brushShader->ID, "u_BrushUV"), uv.x, uv.y);
    m_brushShader->setFloat("u_Radius", radius);
    m_brushShader->setFloat("u_Strength", strength * Time::deltaTime);
    m_brushShader->setInt("u_BrushType", static_cast<int>(type));
    m_brushShader->setFloat("u_TargetHeight", targetHeight);
    
    GL::gl_glBindVertexArray(m_quadMesh->GetVAO());
    GL::gl_glDrawElements(DrawMode::TRIANGLES, 6, IndexType::UNSIGNED_INT, 0);
    
    std::swap(m_fboA, m_fboB);
    std::swap(m_texHeightA, m_texHeightB);
    
    GL::gl_glBindFramebuffer(GL_FrameBufferTarget::GL_FRAMEBUFFER, 0);
}

void Terrain::Update() {

	
	
	bool isMouseDown = (Time::frameCount > 20 && Time::frameCount < 480);
	Vector2 hitUV(0.5f + 0.3f * std::sin(Time::frameCount * 0.05f), 0.5f + 0.3f * std::cos(Time::frameCount * 0.05f));
	
	  if (isMouseDown) {
            if (frameCount < 150) {
                terrain->ModifyHeight(hitUV, 0.15f, 5.0f, Terrain::BrushType::Add);
            } else if (frameCount < 300) {
                terrain->ModifyHeight(hitUV, 0.2f, 10.0f, Terrain::BrushType::Smooth);
            } else {
                terrain->ModifyHeight(hitUV, 0.1f, 10.0f, Terrain::BrushType::Flatten, 2.0f);
            }
        }
	
	
    MeshRenderer* mr = gameObject()->GetComponent<MeshRenderer>();
    if (mr && mr->material) {
        mr->material->setTexture2("u_Heightmap", m_texHeightA);
    }	
	SaveHeightmapToBitmap(terrain->GetHeightmapTexture(), 512, 512, "terrain_heightmap_final.bmp");
}

}
