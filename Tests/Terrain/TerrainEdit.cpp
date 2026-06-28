#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <System/Windows/Application.h>
#include <System/GameObject.hpp>
#include <System/Scene.hpp>
#include <System/Camera.hpp>
#include <System/Light.hpp>
#include "Terrain.hpp"
#include <System/Material.hpp>
#include <System/MeshRenderer.hpp>
#include <System/Time.hpp>
#include <System/Input.hpp>

using namespace System;

const char* terrainVertexShader = R"(
#version 100
attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aUV;

varying vec2 UV;
varying vec3 FragPos;
varying vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D u_Heightmap;

void main() {
    float height = texture2D(u_Heightmap, aUV).r;
    vec3 pos = aPos;
    pos.y = height * 10.0; // Scale height for visibility
    
    gl_Position = projection * view * model * vec4(pos, 1.0);
    UV = aUV;
    FragPos = vec3(model * vec4(pos, 1.0));
    Normal = mat3(model) * aNormal; 
}
)";

const char* terrainFragmentShader = R"(
#version 100
precision highp float;
varying vec2 UV;
varying vec3 FragPos;
varying vec3 Normal;

uniform vec4 color;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    vec3 result = (ambient + diffuse) * color.rgb;
    gl_FragColor = vec4(result, 1.0);
}
)";

int main() {
    std::cout << "Starting Terrain Editor Test..." << std::endl;
    System::Windows::Application::initWindow();
    OpenGLVersion version;
    OpenGLInit(&version);

    Scene::Initialize();

    Shader* terrainShader = Shader::CreateFromSource(terrainVertexShader, terrainFragmentShader);
    if (!terrainShader || terrainShader->ID == 0) {
        std::cerr << "Failed to create terrain shader!" << std::endl;
        return 1;
    }

    GameObject* cameraGO = new GameObject("Main Camera");
    Camera* camera = cameraGO->AddComponent<Camera>();
    camera->isMain = true;
    camera->transform()->SetPosition(Vector3(0.0f, 30.0f, -50.0f));
    camera->transform()->SetRotation(Quaternion::Euler(35.0f, 0.0f, 0.0f));



/*
    GameObject* lightGO = new GameObject("Directional Light");
    Light* light = lightGO->AddComponent<Light>();
    light->type = LightType::Directional;
    light->transform()->SetRotation(Quaternion::Euler(45.0f, 45.0f, 0.0f));
	*/
	
	GameObject* lightGO = new GameObject("Point Light");
    Light* light = lightGO->AddComponent<Light>();
    light->type = LightType::Point;
    light->transform()->SetPosition(Vector3(0.0f, 50.0f, -80.0f));//0.0f, 50.0f, -80.0f));
    light->intensity = 2.0f;
	
	
	
	
	
	

    GameObject* terrainGO = new GameObject("Terrain");
    Terrain* terrain = terrainGO->AddComponent<Terrain>();
    terrain->Initialize(512, 512, 100.0f);

    Material* terrainMat = new Material(terrainShader);
    terrainMat->color = Color(0.3f, 0.7f, 0.3f, 1.0f);
    terrainGO->GetComponent<MeshRenderer>()->material = terrainMat;

    int frameCount = 0;
    while (frameCount < 500) {
        Time::startframe();
        Input::UpdateInputState();

        float t = float(frameCount) * 0.05f;
        Vector2 hitUV(0.5f + 0.3f * std::sin(t), 0.5f + 0.3f * std::cos(t));
        
        if (frameCount < 200) {
            terrain->ModifyHeight(hitUV, 0.2f, 0.1f, Terrain::BrushType::Add);
        } else if (frameCount < 400) {
            terrain->ModifyHeight(hitUV, 0.3f, 0.05f, Terrain::BrushType::Smooth);
        } else {
            terrain->ModifyHeight(hitUV, 0.1f, 0.1f, Terrain::BrushType::Flatten, 0.5f);
        }

        Scene::Run(800, 600); 
        frameCount++;

        if (frameCount % 100 == 0) {
            std::cout << "Frame " << frameCount << std::endl;
        }
    }

    std::cout << "Saving final heightmap and screenshot..." << std::endl;
    terrain->SaveHeightmapToBitmap(terrain->GetHeightmapTexture(), 512, 512, "terrain_heightmap_final.bmp");
    camera->TakeScreenshot("terrain_screenshot.bmp");

    return 0;
}
