#if _WIN32
#include <windows.h>
#else
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif
namespace System::Windows {
    /// <summary>
    /// this will handle the window creation.
    /// </summary>
    class Application {
    private:


#if _WIN32
        static HDC hdc;
#else
       
        struct wl_display* display = NULL;
        struct wl_compositor* compositor = NULL;
        struct wl_surface* wl_surface_ptr = NULL;
        struct wl_egl_window* egl_window = NULL;
        EGLDisplay egl_display = EGL_NO_DISPLAY;
        EGLConfig egl_config;
        EGLContext egl_context = EGL_NO_CONTEXT;
        EGLSurface egl_surface = EGL_NO_SURFACE;
#endif

        static int init;

    public:

        static void initWindow() {
#if _WIN32
#else
           
            display = wl_display_connect(NULL);
            if (!display) {
                fprintf(stderr, "Failed to connect to Wayland display, trying EGL_DEFAULT_DISPLAY\n");
            }

            if (display) {
                struct wl_registry* registry = wl_display_get_registry(display);
                wl_registry_add_listener(registry, &registry_listener, NULL);
                wl_display_dispatch(display);
                wl_display_roundtrip(display);
                if (!compositor) { fprintf(stderr, "Failed to find wl_compositor\n"); return 1; }
                wl_surface_ptr = wl_compositor_create_surface(compositor);
                egl_display = eglGetDisplay((EGLNativeDisplayType)display);
            }
            else {
                egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            }

            eglInitialize(egl_display, NULL, NULL);
            EGLint attribs[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_SURFACE_TYPE, EGL_PBUFFER_BIT | EGL_WINDOW_BIT, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_NONE };
            EGLint num_configs;
            eglChooseConfig(egl_display, attribs, &egl_config, 1, &num_configs);
            eglBindAPI(EGL_OPENGL_API);
            EGLint context_attribs[] = { EGL_CONTEXT_MAJOR_VERSION, 3, EGL_CONTEXT_MINOR_VERSION, 3, EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT, EGL_NONE };
            egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attribs);

            if (wl_surface_ptr) {
                egl_window = wl_egl_window_create(wl_surface_ptr, 800, 600);
                egl_surface = eglCreateWindowSurface(egl_display, egl_config, (EGLNativeWindowType)egl_window, NULL);
            }
            else {
                EGLint pbuffer_attribs[] = { EGL_WIDTH, 800, EGL_HEIGHT, 600, EGL_NONE };
                egl_surface = eglCreatePbufferSurface(egl_display, egl_config, pbuffer_attribs);
            }

            eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);


#endif


        }

        inline static void SwapBuffers() {
#if _WIN32
            ::SwapBuffers(hdc);
#else

            eglSwapBuffers(egl_display, egl_surface);
#endif
        //
        
        }



    public: 


        Application() {

        } 


    };
}