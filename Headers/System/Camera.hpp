#ifndef _SYSTEM_CAMERA_H
#define _SYSTEM_CAMERA_H

#include <vector>
#include <cstdint>
#include <System/Rect.hpp>
#include <System/Component.hpp>
#include <System/Matrix4x4.hpp>
 




namespace System{
    class RenderTexture;
    class Matrix4x4;

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
   // System::Transform transform;

    static std::vector<Camera*> getallCameras();

  
    static Camera* Getmain();
    Camera();
    ~Camera();
    Component* Clone() const override {
        Camera* new_camera = new Camera();
        new_camera->isMain = isMain;
        new_camera->cullingMask = cullingMask;
        new_camera->viewport = viewport;
        new_camera->nearClipPlane = nearClipPlane;
        new_camera->farClipPlane = farClipPlane;
        new_camera->targetDisplay = targetDisplay;
        new_camera->orthographic = orthographic;
        new_camera->active = active;
        new_camera->priority = priority;
        new_camera->targetTexture = targetTexture;
        return new_camera;
    }

    void RenderStart() const; 
}; 
}
#endif