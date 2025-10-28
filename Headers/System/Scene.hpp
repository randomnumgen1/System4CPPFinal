#ifndef _SYSTEM_SCENE_H
#define _SYSTEM_SCENE_H

namespace System {
    class GameObject; // Forward declaration

    class Scene {
    public:
        static GameObject* root;

        static void Initialize();
        static void Shutdown();
    };
}

#endif // _SYSTEM_SCENE_H
