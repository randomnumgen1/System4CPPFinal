#include <System/Components/Terrain.hpp>
#include <System/Graphics/GraphicsHelpers.h>
#include <System/Time.hpp>
#include <System/MeshFilter.hpp>
#include <System/MeshRenderer.hpp>
#include <System/GameObject.hpp>
#include <System/Image.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstring>

namespace System {

const char* terrainBrushVertexShader = R"(
#version 100
attribute vec3 aPos;
attribute vec2 aUV;
varying vec2 UV;
void main() {
    gl_Position = vec4(aPos.x * 2.0, aPos.y * 2.0, 0.0, 1.0);
    UV = aUV;
}
)";

const char* terrainBrushFragmentShader = R"(
#version 100
precision highp float;
varying vec2 UV;
uniform sampler2D u_SrcHeightmap;
uniform vec2 u_BrushUV;
uniform float u_Radius;
uniform float u_Strength;
uniform int u_BrushType;
uniform float u_TargetHeight;

void main() {
    float height = texture2D(u_SrcHeightmap, UV).r;
    float dist = distance(UV, u_BrushUV);
    float influence = smoothstep(u_Radius, u_Radius * 0.5, dist);

    if (u_BrushType == 1) { // Add
        height += influence * u_Strength;
    } else if (u_BrushType == 2) { // Subtract
        height -= influence * u_Strength;
    } else if (u_BrushType == 3) { // Smooth
        vec2 texelSize = vec2(1.0 / 512.0, 1.0 / 512.0);
        float avg = 0.0;
        for(float x = -1.0; x <= 1.0; x += 1.0) {
            for(float y = -1.0; y <= 1.0; y += 1.0) {
                avg += texture2D(u_SrcHeightmap, UV + vec2(x, y) * texelSize).r;
            }
        }
        avg /= 9.0;
        height = mix(height, avg, influence * u_Strength);
    } else if (u_BrushType == 4) { // Flatten
        height = mix(height, u_TargetHeight, influence * u_Strength);
    }

    gl_FragColor = vec4(height, height, height, 1.0);
}
)";

Terrain::Terrain() : m_mapWidth(512), m_mapHeight(512), m_size(100.0f),
                     m_texHeightA(0), m_texHeightB(0), m_fboA(0), m_fboB(0),
                     m_brushShader(nullptr), m_mesh(nullptr), m_quadMesh(nullptr) {
}

Terrain::~Terrain() {
    using namespace Graphics;
    if (m_texHeightA) SYSTEM_INTERNAL_glDeleteTextures(1, &m_texHeightA);
    if (m_texHeightB) SYSTEM_INTERNAL_glDeleteTextures(1, &m_texHeightB);
    if (m_fboA) SYSTEM_INTERNAL_glDeleteFramebuffers(1, &m_fboA);
    if (m_fboB) SYSTEM_INTERNAL_glDeleteFramebuffers(1, &m_fboB);
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

    MeshFilter* mf = gameObject()->GetComponent<MeshFilter>();
    if (!mf) mf = gameObject()->AddComponent<MeshFilter>();
    mf->mesh = m_mesh;

    MeshRenderer* mr = gameObject()->GetComponent<MeshRenderer>();
    if (!mr) mr = gameObject()->AddComponent<MeshRenderer>();
}

void Terrain::SaveHeightmapToBitmap(uint32_t tex, int width, int height, const std::string& filename) {
    using namespace Graphics;
    std::vector<uint8_t> data(width * height * 4);
    uint32_t tempFbo;
    SYSTEM_INTERNAL_glGenFramebuffers(1, &tempFbo);
    SYSTEM_INTERNAL_glBindFramebuffer(0x8D40, tempFbo);
    SYSTEM_INTERNAL_glFramebufferTexture2D(0x8D40, 0x8CE0, 0x0DE1, tex, 0);

    SYSTEM_INTERNAL_glReadPixels(0, 0, width, height, 0x1908, 0x1401, data.data());

    SYSTEM_INTERNAL_glBindFramebuffer(0x8D40, 0);
    SYSTEM_INTERNAL_glDeleteFramebuffers(1, &tempFbo);

    Image img(width, height);
    std::memcpy(img.m_pixels.data(), data.data(), width * height * 4);
    img.Save(filename, Image::BMP);
}

void Terrain::InitHeightmap() {
    using namespace Graphics;
    GL::gl_glGenTextures(1, &m_texHeightA);
    GL::gl_glBindTexture(GLenum1::GL_TEXTURE_2D, m_texHeightA);
    GL::gl_glTexImage2D(0x0DE1, 0, 0x1908, m_mapWidth, m_mapHeight, 0, 0x1908, 0x1401, nullptr);
    GL::gl_glTexParameteri(0x0DE1, 0x2801, 0x2601);
    GL::gl_glTexParameteri(0x0DE1, 0x2800, 0x2601);
    GL::gl_glTexParameteri(0x0DE1, 0x2802, 0x812F);
    GL::gl_glTexParameteri(0x0DE1, 0x2803, 0x812F);

    SYSTEM_INTERNAL_glGenFramebuffers(1, &m_fboA);
    SYSTEM_INTERNAL_glBindFramebuffer(0x8D40, m_fboA);
    SYSTEM_INTERNAL_glFramebufferTexture2D(0x8D40, 0x8CE0, 0x0DE1, m_texHeightA, 0);

    GL::gl_glGenTextures(1, &m_texHeightB);
    GL::gl_glBindTexture(GLenum1::GL_TEXTURE_2D, m_texHeightB);
    GL::gl_glTexImage2D(0x0DE1, 0, 0x1908, m_mapWidth, m_mapHeight, 0, 0x1908, 0x1401, nullptr);
    GL::gl_glTexParameteri(0x0DE1, 0x2801, 0x2601);
    GL::gl_glTexParameteri(0x0DE1, 0x2800, 0x2601);
    GL::gl_glTexParameteri(0x0DE1, 0x2802, 0x812F);
    GL::gl_glTexParameteri(0x0DE1, 0x2803, 0x812F);

    SYSTEM_INTERNAL_glGenFramebuffers(1, &m_fboB);
    SYSTEM_INTERNAL_glBindFramebuffer(0x8D40, m_fboB);
    SYSTEM_INTERNAL_glFramebufferTexture2D(0x8D40, 0x8CE0, 0x0DE1, m_texHeightB, 0);

    SYSTEM_INTERNAL_glBindFramebuffer(0x8D40, m_fboA);
    SYSTEM_INTERNAL_glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    SYSTEM_INTERNAL_glClear(0x00004000);
    SYSTEM_INTERNAL_glBindFramebuffer(0x8D40, m_fboB);
    SYSTEM_INTERNAL_glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    SYSTEM_INTERNAL_glClear(0x00004000);
    SYSTEM_INTERNAL_glBindFramebuffer(0x8D40, 0);
}

void Terrain::InitShaders() {
    auto createShader = [](GLenum type, const char* source) {
        GLuint s = SYSTEM_INTERNAL_glCreateShader(type);
        SYSTEM_INTERNAL_glShaderSource(s, 1, &source, NULL);
        SYSTEM_INTERNAL_glCompileShader(s);
        GLint success;
        SYSTEM_INTERNAL_glGetShaderiv(s, 0x8B81, &success);
        if (!success) {
            char infoLog[512];
            SYSTEM_INTERNAL_glGetShaderInfoLog(s, 512, NULL, infoLog);
            std::cerr << "Brush Shader compilation error: " << infoLog << std::endl;
        }
        return s;
    };

    GLuint v = createShader(0x8B31, terrainBrushVertexShader);
    GLuint f = createShader(0x8B30, terrainBrushFragmentShader);

    m_brushShader = new Shader();
    m_brushShader->ID = SYSTEM_INTERNAL_glCreateProgram();
    SYSTEM_INTERNAL_glAttachShader(m_brushShader->ID, v);
    SYSTEM_INTERNAL_glAttachShader(m_brushShader->ID, f);

    SYSTEM_INTERNAL_glBindAttribLocation(m_brushShader->ID, 0, "aPos");
    SYSTEM_INTERNAL_glBindAttribLocation(m_brushShader->ID, 2, "aUV");

    SYSTEM_INTERNAL_glLinkProgram(m_brushShader->ID);
    GLint success;
    SYSTEM_INTERNAL_glGetProgramiv(m_brushShader->ID, 0x8B82, &success);
    if (!success) {
        char infoLog[512];
        SYSTEM_INTERNAL_glGetProgramInfoLog(m_brushShader->ID, 512, NULL, infoLog);
        std::cerr << "Brush Program linking error: " << infoLog << std::endl;
    }
    SYSTEM_INTERNAL_glDeleteShader(v);
    SYSTEM_INTERNAL_glDeleteShader(f);
}

void Terrain::InitMesh() {
    m_mesh = new Mesh();
    std::vector<Vector3> vertices;
    std::vector<Vector2> uvs;
    std::vector<int> indices;
    int resX = 100;
    int resY = 100;
    float stepX = 1.0f / (float)(resX - 1);
    float stepY = 1.0f / (float)(resY - 1);
    for (int j = 0; j < resY; ++j) {
        for (int i = 0; i < resX; ++i) {
            vertices.push_back(Vector3((float(i) * stepX - 0.5f) * m_size, 0.0f, (float(j) * stepY - 0.5f) * m_size));
            uvs.push_back(Vector2(float(i) * stepX, float(j) * stepY));
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

    SYSTEM_INTERNAL_glDisable(0x0B71);
    SYSTEM_INTERNAL_glDisable(0x0BE2);
    SYSTEM_INTERNAL_glDisable(0x0B44);

    SYSTEM_INTERNAL_glViewport(0, 0, m_mapWidth, m_mapHeight);
    SYSTEM_INTERNAL_glBindFramebuffer(0x8D40, m_fboB);
    SYSTEM_INTERNAL_glUseProgram(m_brushShader->ID);

    SYSTEM_INTERNAL_glActiveTexture(0x84C0);
    SYSTEM_INTERNAL_glBindTexture(0x0DE1, m_texHeightA);
    SYSTEM_INTERNAL_glUniform1i(SYSTEM_INTERNAL_glGetUniformLocation(m_brushShader->ID, "u_SrcHeightmap"), 0);

    SYSTEM_INTERNAL_glUniform2f(SYSTEM_INTERNAL_glGetUniformLocation(m_brushShader->ID, "u_BrushUV"), uv.x, uv.y);
    SYSTEM_INTERNAL_glUniform1f(SYSTEM_INTERNAL_glGetUniformLocation(m_brushShader->ID, "u_Radius"), radius);
    SYSTEM_INTERNAL_glUniform1f(SYSTEM_INTERNAL_glGetUniformLocation(m_brushShader->ID, "u_Strength"), strength);
    SYSTEM_INTERNAL_glUniform1i(SYSTEM_INTERNAL_glGetUniformLocation(m_brushShader->ID, "u_BrushType"), static_cast<int>(type));
    SYSTEM_INTERNAL_glUniform1f(SYSTEM_INTERNAL_glGetUniformLocation(m_brushShader->ID, "u_TargetHeight"), targetHeight);

    SYSTEM_INTERNAL_glBindVertexArray(m_quadMesh->GetVAO());
    SYSTEM_INTERNAL_glBindBuffer(0x8892, m_quadMesh->GetVBO());
    SYSTEM_INTERNAL_glBindBuffer(0x8893, m_quadMesh->GetEBO());

    SYSTEM_INTERNAL_glDrawElements(0x0004, 6, 0x1405, 0);
    SYSTEM_INTERNAL_glBindVertexArray(0);

    std::swap(m_fboA, m_fboB);
    std::swap(m_texHeightA, m_texHeightB);

    SYSTEM_INTERNAL_glBindFramebuffer(0x8D40, 0);
}

void Terrain::Update() {
    MeshRenderer* mr = gameObject()->GetComponent<MeshRenderer>();
    if (mr && mr->material) {
        mr->material->setTexture2("u_Heightmap", m_texHeightA);
    }
}

}
