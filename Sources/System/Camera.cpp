#include <System/Camera.hpp>
#include <vector>
#include <System/RenderTexture.h>
#include <algorithm>
#include <System/Mathf.hpp>
#include <System/Graphics/GraphicsHelpers.h>
#include <climits>
#include <System/Image.hpp>


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

void System::Camera::RenderStart() {
    if (targetTexture == nullptr) {
        // Get window dimensions from the current viewport
        GLint windowViewport[4];
        System::Graphics::GL::gl_glGetIntegerv(GL_VIEWPORT, windowViewport);
        int windowWidth = windowViewport[2];
        int windowHeight = windowViewport[3];

        // Calculate pixel viewport from normalized coordinates
        int vpX = static_cast<int>(viewport.x * windowWidth);
        int vpY = static_cast<int>(viewport.y * windowHeight);
        int vpWidth = static_cast<int>(viewport.width * windowWidth);
        int vpHeight = static_cast<int>(viewport.height * windowHeight);

        // Render to the screen
        System::Graphics::GL::gl_glBindFramebuffer(System::Graphics::GL_FrameBufferTarget::GL_FRAMEBUFFER, 0);
        System::Graphics::GL::gl_glViewport(vpX, vpY, vpWidth, vpHeight);
        System::Graphics::GL::gl_glClear(System::Graphics::GL_BitField::COLOR_BUFFER_BIT | System::Graphics::GL_BitField::DEPTH_BUFFER_BIT);

        // Calculate projection matrix with the correct aspect ratio
        float aspectRatio = (vpHeight > 0) ? static_cast<float>(vpWidth) / static_cast<float>(vpHeight) : 1.0f;
        projectionMatrix = orthographic
            ? System::Matrix4x4::Ortho(vpX, vpX + vpWidth, vpY, vpY + vpHeight, nearClipPlane, farClipPlane)
            : System::Matrix4x4::Perspective(Mathf::Radians(60.0f), aspectRatio, nearClipPlane, farClipPlane);

        viewMatrix = Matrix4x4::LookAt(System::Vector3(0, 0, 5), System::Vector3(0, 0, 0), System::Vector3(0, 1, 0));

    }else{
        //Render to the target texture
        System::Graphics::GL::gl_glBindFramebuffer(System::Graphics::GL_FrameBufferTarget::GL_FRAMEBUFFER, targetTexture->GetNativeTexturePtr());
        System::Graphics::GL::gl_glViewport(0, 0, targetTexture->GetWidth(), targetTexture->GetHeight());
        System::Graphics::GL::gl_glClear(System::Graphics::GL_BitField::COLOR_BUFFER_BIT | System::Graphics::GL_BitField::DEPTH_BUFFER_BIT);

    }
}
void Camera::TakeScreenshot(const std::string& filename) {
    GLint viewport[4];
    System::Graphics::GL::gl_glGetIntegerv(GL_VIEWPORT, viewport);
    int x = viewport[0];
    int y = viewport[1];
    int width = viewport[2];
    int height = viewport[3];

    Image image(width, height);
    System::Graphics::GL::gl_glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, image.m_pixels.data());
    image.Flip();
    image.Save(filename, Image::ImageFormat::BMP);
}
};