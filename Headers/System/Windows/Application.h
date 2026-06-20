#if _WIN32
#include <windows.h>
#else

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
        static struct wl_display* display;
#endif

        static int init;

    public:

        static void initWindow() {





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