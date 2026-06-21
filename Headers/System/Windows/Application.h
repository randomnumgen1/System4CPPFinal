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

extern "C" {
    struct xdg_wm_base;
    struct xdg_surface;
    struct xdg_toplevel;
    extern const struct wl_interface xdg_wm_base_interface;
    extern const struct wl_interface xdg_surface_interface;
    extern const struct wl_interface xdg_toplevel_interface;

    struct xdg_wm_base_listener {
        void (*ping)(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial);
    };
    struct xdg_surface_listener {
        void (*configure)(void* data, struct xdg_surface* xdg_surface, uint32_t serial);
    };
    struct xdg_toplevel_listener {
        void (*configure)(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, struct wl_array* states);
        void (*close)(void* data, struct xdg_toplevel* xdg_toplevel);
    };

    static inline int xdg_wm_base_add_listener(struct xdg_wm_base* xdg_wm_base, const struct xdg_wm_base_listener* listener, void* data) {
        return wl_proxy_add_listener((struct wl_proxy*)xdg_wm_base, (void (**)(void)) listener, data);
    }
    static inline void xdg_wm_base_pong(struct xdg_wm_base* xdg_wm_base, uint32_t serial) {
        wl_proxy_marshal((struct wl_proxy*)xdg_wm_base, 3, serial);
    }
    static inline struct xdg_surface* xdg_wm_base_get_xdg_surface(struct xdg_wm_base* xdg_wm_base, struct wl_surface* surface) {
        return (struct xdg_surface*)wl_proxy_marshal_constructor((struct wl_proxy*)xdg_wm_base, 2, &xdg_surface_interface, NULL, surface);
    }
    static inline int xdg_surface_add_listener(struct xdg_surface* xdg_surface, const struct xdg_surface_listener* listener, void* data) {
        return wl_proxy_add_listener((struct wl_proxy*)xdg_surface, (void (**)(void)) listener, data);
    }
    static inline struct xdg_toplevel* xdg_surface_get_toplevel(struct xdg_surface* xdg_surface) {
        return (struct xdg_toplevel*)wl_proxy_marshal_constructor((struct wl_proxy*)xdg_surface, 1, &xdg_toplevel_interface, NULL);
    }
    static inline void xdg_surface_ack_configure(struct xdg_surface* xdg_surface, uint32_t serial) {
        wl_proxy_marshal((struct wl_proxy*)xdg_surface, 4, serial);
    }
    static inline int xdg_toplevel_add_listener(struct xdg_toplevel* xdg_toplevel, const struct xdg_toplevel_listener* listener, void* data) {
        return wl_proxy_add_listener((struct wl_proxy*)xdg_toplevel, (void (**)(void)) listener, data);
    }

    extern const struct wl_interface wl_surface_interface;
    inline const struct wl_interface* xdg_shell_types[] = {
        NULL, NULL, NULL, NULL, NULL, &xdg_surface_interface, &wl_surface_interface, &xdg_toplevel_interface,
    };
    inline const struct wl_message xdg_wm_base_requests[] = {
        { "destroy", "", xdg_shell_types + 0 },
        { "create_positioner", "n", xdg_shell_types + 0 },
        { "get_xdg_surface", "no", xdg_shell_types + 5 },
        { "pong", "u", xdg_shell_types + 0 },
    };
    inline const struct wl_message xdg_wm_base_events[] = {
        { "ping", "u", xdg_shell_types + 0 },
    };
    inline const struct wl_interface xdg_wm_base_interface = {
        "xdg_wm_base", 1, 4, xdg_wm_base_requests, 1, xdg_wm_base_events,
    };
    inline const struct wl_message xdg_surface_requests[] = {
        { "destroy", "", xdg_shell_types + 0 },
        { "get_toplevel", "n", xdg_shell_types + 7 },
        { "get_popup", "n?oo", xdg_shell_types + 0 },
        { "set_window_geometry", "iiii", xdg_shell_types + 0 },
        { "ack_configure", "u", xdg_shell_types + 0 },
    };
    inline const struct wl_message xdg_surface_events[] = {
        { "configure", "u", xdg_shell_types + 0 },
    };
    inline const struct wl_interface xdg_surface_interface = {
        "xdg_surface", 1, 5, xdg_surface_requests, 1, xdg_surface_events,
    };
    inline const struct wl_message xdg_toplevel_requests[] = {
        { "destroy", "", xdg_shell_types + 0 }, { "set_parent", "?o", xdg_shell_types + 0 },
        { "set_title", "s", xdg_shell_types + 0 }, { "set_app_id", "s", xdg_shell_types + 0 },
        { "show_window_menu", "ouii", xdg_shell_types + 0 }, { "move", "ou", xdg_shell_types + 0 },
        { "resize", "ouu", xdg_shell_types + 0 }, { "set_max_size", "ii", xdg_shell_types + 0 },
        { "set_min_size", "ii", xdg_shell_types + 0 }, { "set_maximized", "", xdg_shell_types + 0 },
        { "unset_maximized", "", xdg_shell_types + 0 }, { "set_fullscreen", "?o", xdg_shell_types + 0 },
        { "unset_fullscreen", "", xdg_shell_types + 0 }, { "set_minimized", "", xdg_shell_types + 0 },
    };
    inline const struct wl_message xdg_toplevel_events[] = {
        { "configure", "iia", xdg_shell_types + 0 },
        { "close", "", xdg_shell_types + 0 },
    };
    inline const struct wl_interface xdg_toplevel_interface = {
        "xdg_toplevel", 1, 14, xdg_toplevel_requests, 2, xdg_toplevel_events,
    };
}
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
        inline static struct xdg_wm_base* xdg_wm_base_ptr = NULL;
        inline static struct xdg_surface* xdg_surface_ptr = NULL;
        inline static struct xdg_toplevel* xdg_toplevel_ptr = NULL;
        inline static EGLDisplay egl_display = EGL_NO_DISPLAY;
        inline static EGLConfig egl_config;
        inline static EGLContext egl_context = EGL_NO_CONTEXT;
        inline static EGLSurface egl_surface = EGL_NO_SURFACE;

        static void xdg_wm_base_ping(void* data, struct xdg_wm_base* xdg_wm_base, uint32_t serial) {
            xdg_wm_base_pong(xdg_wm_base, serial);
        }
        inline static const struct xdg_wm_base_listener wm_base_listener = { xdg_wm_base_ping };

        static void xdg_surface_configure(void* data, struct xdg_surface* xdg_surface, uint32_t serial) {
            xdg_surface_ack_configure(xdg_surface, serial);
        }
        inline static const struct xdg_surface_listener surface_listener = { xdg_surface_configure };

        static void xdg_toplevel_configure(void* data, struct xdg_toplevel* xdg_toplevel, int32_t width, int32_t height, struct wl_array* states) {}
        static void xdg_toplevel_close(void* data, struct xdg_toplevel* xdg_toplevel) {}
        inline static const struct xdg_toplevel_listener toplevel_listener = { xdg_toplevel_configure, xdg_toplevel_close };

        static void registry_handle_global(void* data, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version) {
            fprintf(stderr, "Wayland Registry: %s (name %u, version %u)\n", interface, name, version);
            fflush(stderr);
            if (strcmp(interface, "wl_compositor") == 0) {
                compositor = (struct wl_compositor*)wl_registry_bind(registry, name, &wl_compositor_interface, 1);
            }
            else if (strcmp(interface, "xdg_wm_base") == 0) {
                xdg_wm_base_ptr = (struct xdg_wm_base*)wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
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
                fflush(stderr);
            }
            else {
                fprintf(stderr, "Connected to Wayland display.\n");
                fflush(stderr);
            }

            if (display) {
                struct wl_registry* registry = wl_display_get_registry(display);
                wl_registry_add_listener(registry, &registry_listener, NULL);
                wl_display_roundtrip(display);
                wl_display_roundtrip(display);
                if (!compositor) { fprintf(stderr, "Failed to find wl_compositor\n"); fflush(stderr); return; }
                else { fprintf(stderr, "Found wl_compositor.\n"); fflush(stderr); }
                if (xdg_wm_base_ptr) {
                    fprintf(stderr, "Found xdg_wm_base.\n"); fflush(stderr);
                    xdg_wm_base_add_listener(xdg_wm_base_ptr, &wm_base_listener, NULL);
                }
                else { fprintf(stderr, "Failed to find xdg_wm_base.\n"); fflush(stderr); }

                wl_surface_ptr = wl_compositor_create_surface(compositor);
                egl_display = eglGetDisplay((EGLNativeDisplayType)display);
            }
            else {
                egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
            }

            if (!eglInitialize(egl_display, NULL, NULL)) {
                fprintf(stderr, "Failed to initialize EGL\n");
                fflush(stderr);
                return;
            }
            else {
                fprintf(stderr, "EGL initialized successfully.\n");
                fflush(stderr);
            }

#if defined(__arm__) || defined(__aarch64__)
            eglBindAPI(EGL_OPENGL_ES_API);
            EGLint render_bit = EGL_OPENGL_ES3_BIT;
#else
            eglBindAPI(EGL_OPENGL_API);
            EGLint render_bit = EGL_OPENGL_BIT;
#endif

            EGLint num_configs;
            bool config_found = false;

            // List of attribute sets to try, from most preferred to least
            EGLint attribs1[] = { EGL_RENDERABLE_TYPE, render_bit, EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_NONE };
            EGLint attribs2[] = { EGL_RENDERABLE_TYPE, render_bit, EGL_SURFACE_TYPE, EGL_WINDOW_BIT | EGL_PBUFFER_BIT, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_DEPTH_SIZE, 16, EGL_NONE };
            EGLint attribs3[] = { EGL_RENDERABLE_TYPE, render_bit, EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_NONE };
            EGLint* attrib_lists[] = { attribs1, attribs2, attribs3, NULL };

            for (int i = 0; attrib_lists[i] != NULL; ++i) {
                if (eglChooseConfig(egl_display, attrib_lists[i], &egl_config, 1, &num_configs) && num_configs > 0) {
                    config_found = true;
                    fprintf(stderr, "EGL config chosen (set %d).\n", i);
                    fflush(stderr);
                    break;
                }
            }

            if (!config_found) {
                fprintf(stderr, "Failed to choose any EGL config. Error: %x\n", eglGetError());
                fflush(stderr);
#if defined(__arm__) || defined(__aarch64__)
                // Last ditch effort for ES2
                EGLint es2_attribs[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_NONE };
                if (eglChooseConfig(egl_display, es2_attribs, &egl_config, 1, &num_configs) && num_configs > 0) {
                    config_found = true;
                    fprintf(stderr, "EGL config chosen (Last ditch ES2).\n");
                    fflush(stderr);
                }
#endif
            }

#if defined(__arm__) || defined(__aarch64__)
            EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
#else
            EGLint context_attribs[] = { EGL_CONTEXT_MAJOR_VERSION, 3, EGL_CONTEXT_MINOR_VERSION, 3, EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT, EGL_NONE };
#endif
            egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attribs);

            if (wl_surface_ptr) {
                if (xdg_wm_base_ptr) {
                    xdg_surface_ptr = xdg_wm_base_get_xdg_surface(xdg_wm_base_ptr, wl_surface_ptr);
                    xdg_surface_add_listener(xdg_surface_ptr, &surface_listener, NULL);
                    xdg_toplevel_ptr = xdg_surface_get_toplevel(xdg_surface_ptr);
                    xdg_toplevel_add_listener(xdg_toplevel_ptr, &toplevel_listener, NULL);
                    wl_surface_commit(wl_surface_ptr);
                    wl_display_roundtrip(display);
                    fprintf(stderr, "Wayland xdg_surface and xdg_toplevel created.\n");
                    fflush(stderr);
                }
                wl_surface_commit(wl_surface_ptr);
                egl_window = wl_egl_window_create(wl_surface_ptr, 800, 600);
                egl_surface = eglCreateWindowSurface(egl_display, egl_config, (EGLNativeWindowType)egl_window, NULL);
                if (egl_surface == EGL_NO_SURFACE) {
                    fprintf(stderr, "Failed to create EGL window surface. Error: %x\n", eglGetError());
                    fflush(stderr);
                }
                else {
                    fprintf(stderr, "EGL window surface created.\n");
                    fflush(stderr);
                }
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
            if (display) {
                wl_display_flush(display);
                wl_display_dispatch_pending(display);
            }
#endif
        }



    public:


        Application() {

        }


    };
}