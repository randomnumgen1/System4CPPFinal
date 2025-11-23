#ifndef _SYSTEM_CAMERA_H
#define _SYSTEM_CAMERA_H

#include <vector>
#include <System/RenderTexture.h>
#include <System/Rect.hpp>
#include <System/Mathf.hpp>
#include <System/Graphics/GraphicsHelpers.h>
#include <System/Matrix4x4.hpp>
#include <System/Transform.hpp>
#include <algorithm>
namespace System{
class Camera : public System::Component {
    static std::vector<Camera*> allCameras;
public:

    Matrix4x4 viewMatrix;
    Matrix4x4 projectionMatrix;


    bool isMain;
    uint32_t cullingMask;
    System::Rect viewport;
    float nearClipPlane;
    float farClipPlane;
    int targetDisplay;
    bool orthographic;
    bool active;
    int priority;
    System::RenderTexture* targetTexture;
    System::Transform transform;

    static std::vector<Camera*> getallCameras();

  
    static Camera* Getmain();
    Camera();
    ~Camera();


    void RenderStart() const; 
};
std::vector<Camera*> Camera::allCameras;
}
#endif