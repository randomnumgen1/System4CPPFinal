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
        static int init;
        static void initWindow() {





        }

        static void SwapBuffers() {
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