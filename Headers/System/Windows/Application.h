#if _WIN32
#include <windows.h>
#else
#include <wayland-client.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
//#include <GLES3/gl3.h>
#include <string.h>
#include <stdio.h>
#include <System/Internal/InternalGLloader.h>
#endif
namespace System::Windows {
    /// <summary>
    /// this will handle the window creation.
    /// </summary>
    class Application {
    private:


#if _WIN32
    public:
        inline static HDC hdc = NULL;
    private:
#else

        inline static struct wl_display* display = NULL;
        inline static struct wl_compositor* compositor = NULL;
        inline static struct wl_surface* wl_surface_ptr = NULL;
        inline static struct wl_egl_window* egl_window = NULL;
        inline static EGLDisplay egl_display = EGL_NO_DISPLAY;
        inline static EGLConfig egl_config;
        inline static EGLContext egl_context = EGL_NO_CONTEXT;
        inline static EGLSurface egl_surface = EGL_NO_SURFACE;

        static void registry_handle_global(void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version) {
            if (strcmp(interface, "wl_compositor") == 0) {
                compositor = (struct wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 1);
            }
        }
        static void registry_handle_global_remove(void* data, struct wl_registry* registry, uint32_t name) {}
        inline static const struct wl_registry_listener registry_listener = { registry_handle_global, registry_handle_global_remove };
#endif

        inline static int init = 0;

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
                if (!compositor) { fprintf(stderr, "Failed to find wl_compositor\n"); return; }
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

            OpenGLVersion Version;
            OpenGLInit(&Version);


#endif


        }

        inline static void SwapBuffers() {
#if _WIN32
            if (hdc) ::SwapBuffers(hdc);
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