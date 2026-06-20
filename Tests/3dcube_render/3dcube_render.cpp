
#include <iostream>
#include <System/Profiler.hpp>
#include <System/Environment/Environment.hpp>
#include <System/Vector3.hpp>
#include <System/Random.h>
#include <System/Graphics/GraphicsHelpers.h> 
#include <System/Color.hpp>
#include <System/Tools/Mapping.hpp>
#include <System/Tools/SoftwareCanvas.h>


#include <System/Input.hpp>


#include <string>
#include <System/IO/File.h>
#include <System/Devices/Gamepad.hpp>


 
#include <System/GameObject.hpp>
#include <System/Scene.hpp>
#include <System/AudioSource.hpp>
#include <System/AudioListener.hpp>
#include <System/AudioClip.hpp>

 
#include <thread>
#include <System/Camera.hpp>
#include <System/MeshRenderer.hpp>
#include <System/Transform.hpp>
#include <System/Light.hpp>
#include <System/Time.hpp>

void ConfineCursor(HWND hwnd)
{
    RECT rect;
    GetClientRect(hwnd, &rect);

    POINT tl = { rect.left, rect.top };
    POINT br = { rect.right, rect.bottom };

    ClientToScreen(hwnd, &tl);
    ClientToScreen(hwnd, &br);

    rect.left = tl.x;
    rect.top = tl.y;
    rect.right = br.x;
    rect.bottom = br.y;

    SetForegroundWindow(hwnd); // required
    ClipCursor(&rect);
}
 System::Vector3 posit = System::Vector3(0.0f, 0.0f, 0.0f);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_ACTIVATE:
        if (wParam != WA_INACTIVE)
            ConfineCursor(hwnd);
        else
            ClipCursor(nullptr);
        break; 
    case WM_KEYDOWN:
        System::Input::UpdateKeyState((uint32_t)wParam, true);
     //   std::cout << "key down" << wParam << std::endl;
        break;

    case WM_KEYUP:
        System::Input::UpdateKeyState((uint32_t)wParam, false);
        break;

    case WM_MOUSEMOVE:
    {
        // LOWORD = x, HIWORD = y
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);
       // System::Input::UpdateMousePosition(System::Vector2((float)x, (float)y));
        break;
    }

    case WM_MOUSEWHEEL:
    {
        short delta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (delta > 0)
            System::Input::UpdateKeyState(VK_MBUTTON, true); // or map to WheelUp
        else
            System::Input::UpdateKeyState(VK_MBUTTON, false); // or map to WheelDown
        break;
    }
    case WM_INPUT:
    {
        UINT size = 0;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

        BYTE* buffer = new BYTE[size];
        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER)) == size) {
            RAWINPUT* raw = (RAWINPUT*)buffer;

            if (raw->header.dwType == RIM_TYPEMOUSE) {
                LONG dx = raw->data.mouse.lLastX;
                LONG dy = raw->data.mouse.lLastY;

                System::Input::UpdateRawMouseDelta(dx, dy);
            }
        }
        delete[] buffer;
        break;
    }
    case WM_DESTROY:
      //  PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
int madfsdin();
HDC hdc = NULL;
int screenW = 0;
int screenH = 0;

void dfsfsddss() {
     screenW = GetSystemMetrics(SM_CXSCREEN);
     screenH = GetSystemMetrics(SM_CYSCREEN);




    WNDCLASSA wc = { 0 };
    wc.lpfnWndProc = WndProc; // Default window procedure
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "SampleWindowClass";

    RegisterClassA(&wc);

    HWND dummyWin = CreateWindowA(wc.lpszClassName, "Sample Window", 0, 200, 200, 300, 300, 0, 0, wc.hInstance, 0);
    HDC dummy_dc = GetDC(dummyWin);

    auto pfd_flags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(pfd),
        1, /* version */
        pfd_flags,
        PFD_TYPE_RGBA, /* ipixel type */
        24, /* color bits */
        0, 0, 0, 0, 0, 0,
        8, /* alpha bits */
        0, 0, 0, 0, 0, 0,
        32, /* depth bits */
        8, /* stencil bits */
        0,
        PFD_MAIN_PLANE, /* Layer type */
        0, 0, 0, 0
    };
    int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
    SetPixelFormat(dummy_dc, pixel_format, &pfd);

    HGLRC dummy_context = wglCreateContext(dummy_dc);
    wglMakeCurrent(dummy_dc, dummy_context);

    OpenGLVersion Version;
    OpenGLInit(&Version);

    wglMakeCurrent(dummy_dc, 0);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummyWin, dummy_dc);
    DestroyWindow(dummyWin);





    /*
    HWND hwnd = CreateWindowA(wc.lpszClassName, "Sample Window",
                              0,
                              0, 0, 800, 600,
                              NULL, NULL, wc.hInstance, NULL);
*/
    HWND hwnd = CreateWindowA(
        wc.lpszClassName,
        "Sample Window",
        WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,          // borderless
        0,
        0,
        screenW,           // full screen width
        screenH,           // full screen height
        NULL,
        NULL,
        wc.hInstance,
        NULL
    );
    hdc = GetDC(hwnd);






    RAWINPUTDEVICE rid = {};
    rid.usUsagePage = 0x01; // Generic desktop controls
    rid.usUsage = 0x02;     // Mouse
    rid.dwFlags = 0; //RIDEV_INPUTSINK Receive input even when not focused (optional)
    rid.hwndTarget = hwnd;

    RegisterRawInputDevices(&rid, 1, sizeof(rid));











    PIXELFORMATDESCRIPTOR pfd2 = {};
    pfd2.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd2.nVersion = 1;
    pfd2.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ;
    pfd2.iPixelType = PFD_TYPE_RGBA;
    pfd2.cColorBits = 32;
    pfd2.cDepthBits = 24;
    pfd2.cStencilBits = 8;
    pfd2.iLayerType = PFD_MAIN_PLANE;



    int context_attribs[] = {
     0x2091, 4, // WGL_CONTEXT_MAJOR_VERSION_ARB
     0x2092, 6, // WGL_CONTEXT_MINOR_VERSION_ARB
     0x9126, 0x00000001, // WGL_CONTEXT_PROFILE_MASK_ARB = CORE
     0
    };
    int attribs[] = {
      0x2001, 0x0001,        // WGL_DRAW_TO_WINDOW_ARB, GL_TRUE
      0x2010, 0x0001,        // WGL_SUPPORT_OPENGL_ARB, GL_TRUE
      0x2011, 0x0001,        // WGL_DOUBLE_BUFFER_ARB, GL_TRUE
      0x2013, 0x202B,        // WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB
      0x2014, 0x20,          // WGL_COLOR_BITS_ARB, 0x20 (32)
      0x2022, 0x18,          // WGL_DEPTH_BITS_ARB, 0x18 (24)
      0x2023, 0x08,          // WGL_STENCIL_BITS_ARB, 0x08 (8)
      0x0
    };




    int pixel_format2;
    UINT num_formats;
    wglChoosePixelFormatARB(hdc, attribs, 0, 1, &pixel_format2, &num_formats);
    if (!num_formats) {
        printf("Failed to create a pixel format for WGL.\n");
    }
    if (!SetPixelFormat(hdc, pixel_format2, &pfd2)) {
        printf("SetPixelFormat failed for real window.\n");
        return;
    }

    HGLRC ctx = (HGLRC)wglCreateContextAttribsARB(hdc, NULL, context_attribs);
    wglMakeCurrent(hdc, ctx);
    OpenGLInit(&Version);
 

    ShowWindow(hwnd, SW_SHOW);
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~WS_OVERLAPPEDWINDOW;
    style |= WS_POPUP;
    SetWindowLong(hwnd, GWL_STYLE, style);

    SetWindowPos(hwnd, HWND_TOP,
                 0, 0, screenW, screenH,
                 SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    UpdateWindow(hwnd);
   if (wglSwapIntervalEXT) {
      // wglSwapIntervalEXT(1);   // Disable VSync
       wglSwapIntervalEXT(0);   // Disable VSync

    }
    else {
        printf("wglSwapIntervalEXT not supported.\n");
    }
  ShowCursor(FALSE);
 
 


    const GLubyte* ver = System::Graphics::GL::gl_glGetString(0x1F02); // GL_VERSION
    const GLubyte* rend = System::Graphics::GL::gl_glGetString(0x1F01); // GL_RENDERER
    const GLubyte* vend = System::Graphics::GL::gl_glGetString(0x1F00); // GL_VENDOR 

    std::cout << "GL_VERSION:  " << (ver ? (const char*)ver : "null") << "\n";
    std::cout << "GL_RENDERER: " << (rend ? (const char*)rend : "null") << "\n";
    std::cout << "GL_VENDOR:   " << (vend ? (const char*)vend : "null") << "\n";

  //  auto extStr = (const char*)wglGetExtensionsStringEXT();
  //  std::cout << "WGL Extensions: " << (extStr ? extStr : "null") << "\n";

}













// Function from previous example
HGLRC CreateOpenGLContext(HDC hdc)
{
    // Step 1: Set pixel format
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
   // pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ;
    pfd.dwFlags =
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER |
        PFD_SUPPORT_COMPOSITION;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (pixelFormat == 0 || !SetPixelFormat(hdc, pixelFormat, &pfd)) {
        MessageBoxA(NULL, "Failed to set pixel format!", "Error", MB_OK | MB_ICONERROR);
        return NULL;
    }

    // Step 2: Create dummy legacy context
    HGLRC dummyCtx = wglCreateContext(hdc);
    if (!dummyCtx || !wglMakeCurrent(hdc, dummyCtx)) {
        MessageBoxA(NULL, "Failed to create legacy OpenGL context!", "Error", MB_OK | MB_ICONERROR);
        return NULL;
    }

    // Step 3: Load wglCreateContextAttribsARB
    OpenGLVersion Version;
    OpenGLInit(&Version);
    //  auto wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wglCreateContextAttribsARB) {
        MessageBoxA(NULL, "wglCreateContextAttribsARB not available!", "Error", MB_OK | MB_ICONERROR);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(dummyCtx);
        return NULL;
    }
     
    // Step 4: Create modern context
    int attribs[] = {
        0x2091, 4, // WGL_CONTEXT_MAJOR_VERSION_ARB
        0x2092, 6, // WGL_CONTEXT_MINOR_VERSION_ARB
        0x9126, 0x00000001, // WGL_CONTEXT_PROFILE_MASK_ARB = CORE
        0
    };

    HGLRC modernCtx = wglCreateContextAttribsARB(hdc, 0, attribs);
    if (!modernCtx) {
        MessageBoxA(NULL, "Failed to create modern OpenGL context!", "Error", MB_OK | MB_ICONERROR);
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(dummyCtx);
        return NULL;
    }

    // Step 5: Clean up dummy context
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(dummyCtx);
    wglMakeCurrent(hdc, modernCtx);

    const GLubyte* ver = System::Graphics::GL::gl_glGetString(0x1F02); // GL_VERSION
    const GLubyte* rend = System::Graphics::GL::gl_glGetString(0x1F01); // GL_RENDERER
    const GLubyte* vend = System::Graphics::GL::gl_glGetString(0x1F00); // GL_VENDOR 

    std::cout << "GL_VERSION:  " << (ver ? (const char*)ver : "null") << "\n";
    std::cout << "GL_RENDERER: " << (rend ? (const char*)rend : "null") << "\n";
    std::cout << "GL_VENDOR:   " << (vend ? (const char*)vend : "null") << "\n";









    return modernCtx;
}
float test = -5.0f;



/*
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN:
        test += 0.1f;
        System::Input::UpdateKeyState(wParam, true);
       /// MessageBoxA(NULL, "Failed to make OpenGL context current!", "Error", MB_OK | MB_ICONERROR);
        // System::Input::UpdateInputState
        switch (wParam) {
        case 'W': // forward (positive Z or Y depending on your convention)
            posit.z += 0.1f;
            break;
        case 'S': // backward
            posit.z -= 0.1f;
            break;
        case 'A': // left
            posit.x -= 0.1f;
            break;
        case 'D': // right
            posit.x += 0.1f;
            break;
        case VK_SPACE: // jump up
            posit.y += 0.1f;
            break;
        case VK_SHIFT: // move down
            posit.y -= 0.1f;
            break;
        }
        break;
    case WM_KEYUP:
        System::Input::UpdateKeyState(wParam, false);
      //  MessageBoxA(NULL, "Failed to make OpenGL context current!", "Error", MB_OK | MB_ICONERROR);

        break;
        // Add more cases as needed
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
*/
float horizontalSpeed = 2.0f;

float yaw = 0.0f;
float pitch = 0.0f;
float speedH = 2.0f;
float speedV = 2.0f;
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previnstance, LPSTR cmdline, int cmdshow){
  unsigned int currentControl;
    _controlfp_s(&currentControl, _PC_24, _MCW_PC);
    _controlfp_s(&currentControl, _RC_NEAR, _MCW_RC);

    /*
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
   std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
*/

     
    dfsfsddss();

 
     



    System::Devices::GamePad pad;

   System::Scene::Initialize();
   // OpenGLVersion version;
   // OpenGLInit(&version);
    System::GameObject* cameraGO = new System::GameObject("Main Camera");
    System::Camera* camera = cameraGO->AddComponent<System::Camera>();
    camera->isMain = true;
   
    

    camera->transform()->SetPosition(System::Vector3(0.0f, 0.0f, -5.0f));
    camera->transform()->SetRotation(System::Quaternion::Euler(0, 20, 0));
    //camera->transform()->SetRotation(System::Quaternion::Euler(0, 5, 0));

     System::Matrix4x4 worldToCamera = camera->GetworldToCameraMatrix();
     std::cout << worldToCamera.ToString() << std::endl;


     

    System::GameObject* lightGO = new System::GameObject("Light");
    System::Light* light = lightGO->AddComponent<System::Light>();
    light->type = System::LightType::Point;
    light->color = System::Color32(255, 255, 255, 255);
    light->intensity = 6.0f;
    light->range = 2.0f;

    light->transform()->SetPosition(System::Vector3(0.0f, 0.0f, -1.4f));





    System::GameObject* cube = System::GameObject::CreatePrimitive(System::PrimitiveType::Cube);
    System::MeshRenderer* renderer = cube->GetComponent<System::MeshRenderer>(); 
#if defined(__arm__) || defined(__aarch64__)
    System::Shader* shader = new System::Shader("shaderbasicflat_es.vert", "shaderbasicflat_es.frag");
#else	
    System::Shader* shader = new System::Shader("shaderadvanced.vert", "shaderadvanced.frag");
   // System::Shader* shader = new System::Shader("shaderadvanced.vert", "shaderadvanced.frag", "shaderadvanced.geo");

//System::Shader* shader = new System::Shader("shaderbasicflat.vert", "shaderbasicflat.frag");
#endif	

    System::Material* material = new System::Material(shader);
    material->color = System::Color(1, 0, 0, 1);

    renderer->material = material;

    /**/
    System::GameObject* cube2 = System::GameObject::CreatePrimitive(System::PrimitiveType::Sphere);
    System::MeshRenderer* renderer2 = cube2->GetComponent<System::MeshRenderer>();
    renderer2->material = material;
   // cube2->transform->SetPosition(System::Vector3(-1, 0, 0));
    cube2->transform->SetPosition(System::Vector3(-1.0f, 0.0f, -1.0f));


    System::Scene scene;
   

    std::cout << "camera rendering at" << camera->transform()->GetPosition().ToString() << std::endl;


    std::cout << "Camera Pos: " << camera->transform()->GetPosition().ToString() << std::endl;
    std::cout << "Light Pos: " << System::Light::allLights[0]->transform()->GetPosition().ToString() << std::endl;

 

    // Example rendering loop
 

 
    MSG msg = {};
    while (true) {
        System::Time::startframe();
       






        // Process all pending messages
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
scene.Run(screenW, screenH);

bool fastMode = System::Input::GetKey(System::KeyCode::LeftShift);
float movementSpeed = fastMode ? 10.0f : 1.0f;


        


 
//yaw += speedH * System::Input::GetAxis("MouseX");
//pitch += speedV * System::Input::GetAxis("MouseY");




float mouseX = System::Input::GetRawMouseDeltaX();
float mouseY = System::Input::GetRawMouseDeltaY();

yaw += 100.0f * mouseX * System::Time::deltaTime;
pitch += 100.0f * mouseY * System::Time::deltaTime;


if (pitch > 89.0f) pitch = 89.0f;
if (pitch < -89.0f) pitch = -89.0f;



//std::cout << "mousex:" << (horizontalSpeed * System::Input::GetAxis("MouseX"));

//camera->transform()->Rotate(v, h, 0,Space::Self); 
camera->transform()->seteulerAngles(System::Vector3(pitch, yaw, 0.0f));










System::Vector3 move = System::Vector3::zero;

if (System::Input::GetKey(System::KeyCode::W)) {
    move += camera->transform()->forward();
}
if (System::Input::GetKey(System::KeyCode::S)) {
    move -= camera->transform()->forward();
}
if (System::Input::GetKey(System::KeyCode::A)) {
    move -= camera->transform()->right();

}
if (System::Input::GetKey(System::KeyCode::D)) {
    move += camera->transform()->right();
}
// world movement

           // Move the camera forward in world space (ignores camera rotation)
if (System::Input::GetKey(System::KeyCode::I)) {
    move += System::Vector3::forward;
}
// Move the camera backward in world space (ignores camera rotation)
if (System::Input::GetKey(System::KeyCode::K)) {
    move += System::Vector3::back;
}
// Move the camera left in world space (ignores camera rotation)
if (System::Input::GetKey(System::KeyCode::J)) {
    move += System::Vector3::left;
}
// Move the camera right in world space (ignores camera rotation)
if (System::Input::GetKey(System::KeyCode::L)) {
    move += System::Vector3::right;
}

if (move != System::Vector3::zero) {
    camera->transform()->SetPosition(camera->transform()->GetPosition() + move.normalized() * movementSpeed * System::Time::deltaTime);
}





















if (System::Input::GetKeyDown(System::KeyCode::X)) {
    PostQuitMessage(0);
   // std::cout << "delta time" << System::Time::deltaTime<< std::endl;
    posit.z += 1.0f * System::Time::deltaTime;
}
 

 
 
 


        // --- Game update & render ---
        pad.update();
        System::Input::UpdateInputState();
        
         

      //  camera->transform()->SetPosition(posit);
        //  camera->transform()->SetPosition(System::Vector3(0, 0, test));
       // camera->transform()->SetLocalPosition(System::Vector3(test,0,  -5.0f));

        
        SwapBuffers(hdc);
System::Time::endframe();
    }

 
    // Cleanup
   // wglMakeCurrent(NULL, NULL);
   // wglDeleteContext(hglrc);
   // ReleaseDC(hwnd, hdc);

    return 0;
}
 