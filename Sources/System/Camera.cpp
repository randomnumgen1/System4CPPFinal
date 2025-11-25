#include <System/Camera.hpp>
#include <vector>
#include <System/RenderTexture.h>
#include <algorithm>
#include <climits>
#include <System/Mathf.hpp>
#include <System/Graphics/GraphicsHelpers.h>



namespace System {
std::vector<Camera*> Camera::allCameras;
std::vector<System::Camera*> System::Camera::getallCameras(){
    std::vector<Camera*> activeCameras;
    for (const auto& camera : allCameras) {
        if (camera && camera->active) {
            activeCameras.push_back(camera);
        }
    }
    std::sort(activeCameras.begin(), activeCameras.end(),
              [](const Camera* a, const Camera* b) {
                  return a->priority < b->priority;
              });
    return activeCameras;
}
System::Camera* System::Camera::Getmain(){
    Camera* selectedCamera = nullptr;
    int lowestPriority = INT_MAX;
    for (auto& camera : allCameras) {
        if ((camera->isMain) && (camera->active == true)) {
            if (camera->priority < lowestPriority) {
                lowestPriority = camera->priority;
                selectedCamera = camera;
            }
        }
    }
    return selectedCamera;
}
System::Camera::Camera() {
    isMain = false;
    cullingMask = ~0;
    viewport = System::Rect{ 0.0f,0.0f,1.0f,1.0f };
    nearClipPlane = 0.3f;
    farClipPlane = 1000.0f;
    targetDisplay = 0;
    orthographic = false;
    active = true;
    priority = -1;
    targetTexture = nullptr;
    allCameras.push_back(this);


}
System::Camera::~Camera() {
    allCameras.erase(std::remove(allCameras.begin(), allCameras.end(), this), allCameras.end());
}

void System::Camera::RenderStart() const{
    if (targetTexture == nullptr) {
        // Render to the screen
        System::Graphics::GL::gl_glBindFramebuffer(System::Graphics::GL_FrameBufferTarget::GL_FRAMEBUFFER, 0);// render to the screen (This is done by using 0 as the second parameter of glBindFramebuffer).
        System::Graphics::GL::gl_glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        System::Graphics::GL::gl_glClear(System::Graphics::COLOR_BUFFER_BIT | System::Graphics::GL_BitField::DEPTH_BUFFER_BIT);
        System::Matrix4x4 projection = orthographic
            ? System::Matrix4x4::Ortho(viewport.x, viewport.x + viewport.width, viewport.y, viewport.y + viewport.height, nearClipPlane, farClipPlane)
            : System::Matrix4x4::Perspective(Mathf::Radians(60.0f), viewport.width / viewport.height, nearClipPlane, farClipPlane);

        System::Matrix4x4 view = Matrix4x4::LookAt(System::Vector3(0, 0, 5), System::Vector3(0, 0, 0), System::Vector3(0, 1, 0));
        int shaderProgram = 0;
        System::Graphics::GL::gl_glUseProgram(shaderProgram);
        System::Graphics::GL::gl_glUniformMatrix4fv(System::Graphics::GL::gl_glGetUniformLocation(shaderProgram, "projection"), 1, false, &projection.m00);
        System::Graphics::GL::gl_glUniformMatrix4fv(System::Graphics::GL::gl_glGetUniformLocation(shaderProgram, "view"), 1, false, &view.m00);

    }else{
        //Render to the target texture
        System::Graphics::GL::gl_glBindFramebuffer(System::Graphics::GL_FrameBufferTarget::GL_FRAMEBUFFER, targetTexture->GetNativeTexturePtr());
        System::Graphics::GL::gl_glViewport(0, 0, targetTexture->GetWidth(), targetTexture->GetHeight());
        System::Graphics::GL::gl_glClear(System::Graphics::COLOR_BUFFER_BIT | System::Graphics::DEPTH_BUFFER_BIT);

    }
}
};