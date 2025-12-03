#include <System/Camera.hpp>
#include <vector>
#include <System/RenderTexture.h>
#include <algorithm>
#include <System/Mathf.hpp>
#include <System/Graphics/GraphicsHelpers.h>
#include <climits>
#include <System/Image.hpp>
#include <System/Transform.hpp>

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

void System::Camera::RenderStart(int windowWidth, int windowHeight) {
    if (targetTexture == nullptr) {
        // Render to the screen
       System::Graphics::GL::gl_glDisable(System::Graphics::GraphicsCapability::CullFace);
// render to the screen (This is done by using 0 as the second parameter of glBindFramebuffer).
        System::Graphics::GL::gl_glBindFramebuffer(System::Graphics::GL_FrameBufferTarget::GL_FRAMEBUFFER, 0);// render to the screen (This is done by using 0 as the second parameter of glBindFramebuffer).

        int pixelX = static_cast<int>(viewport.x * windowWidth);
        int pixelY = static_cast<int>(viewport.y * windowHeight);
        int pixelWidth = static_cast<int>(viewport.width * windowWidth);
        int pixelHeight = static_cast<int>(viewport.height * windowHeight);

        // Set the viewport
        System::Graphics::GL::gl_glViewport(pixelX, pixelY, pixelWidth, pixelHeight);
        // SYSTEM_INTERNAL_glClearColor(0.0f, 1.0f, 0.0f, 0.5f); 
        // 
        System::Graphics::GL::gl_glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
 System::Graphics::GL::gl_glFrontFace(System::Graphics::WindingOrder::CW);
        // Clear the screen
        System::Graphics::GL::gl_glClear(System::Graphics::GL_BitField::COLOR_BUFFER_BIT | System::Graphics::GL_BitField::DEPTH_BUFFER_BIT);

        float aspect = static_cast<float>(pixelWidth) / static_cast<float>(pixelHeight);
        
        projectionMatrix = orthographic
            ? System::Matrix4x4::Ortho(viewport.x, viewport.x + viewport.width, viewport.y, viewport.y + viewport.height, nearClipPlane, farClipPlane)
            : System::Matrix4x4::Perspective(60.0f, aspect, nearClipPlane, farClipPlane);




       // projectionMatrix = System::Matrix4x4::Perspective( 60.0f   , 1.0f, 1.0f, 100.0f);
        // Calculate the view matrix based on the camera's transform
        
        viewMatrix = Matrix4x4::LookAt(transform()->GetPosition(), transform()->GetPosition() + transform()->forward(), transform()->up());

         


    }else{
        //Render to the target texture
        System::Graphics::GL::gl_glBindFramebuffer(System::Graphics::GL_FrameBufferTarget::GL_FRAMEBUFFER, targetTexture->GetNativeTexturePtr());
        System::Graphics::GL::gl_glViewport(0, 0, targetTexture->GetWidth(), targetTexture->GetHeight());
        System::Graphics::GL::gl_glClear(System::Graphics::GL_BitField::COLOR_BUFFER_BIT | System::Graphics::GL_BitField::DEPTH_BUFFER_BIT);

    }
}
void Camera::TakeScreenshot(const std::string& filename) {
    GLint viewport[4];
    System::Graphics::GL::gl_glGetIntegerv(System::Graphics::GLStateParam::VIEWPORT, viewport);
    int x = viewport[0];
    int y = viewport[1];
    int width = viewport[2];
    int height = viewport[3];

    Image image(width, height);
    System::Graphics::GL::gl_glReadPixels(x, y, width, height, System::Graphics::PixelFormat::RGBA, System::Graphics::PixelType::UNSIGNED_BYTE, image.m_pixels.data());
    image.Flip();
    image.Save(filename, Image::ImageFormat::BMP);
}
Matrix4x4 Camera::GetworldToCameraMatrix() {
    // Get the camera's world transform
    Matrix4x4 localToWorld = transform()->GetLocalToWorldMatrix();

    // Invert it to go from world space to camera space
    Matrix4x4 worldToCamera = localToWorld.inverse();

    // Adjust for OpenGL convention: forward = -Z
    // If your engine defines forward as +Z, you need to flip here.
    // One way: multiply by a correction matrix that flips Z.
    Matrix4x4 correction = Matrix4x4::identity;
    correction.m22 = -1.0f; // flip Z axis

    return correction * worldToCamera;
}

};