#ifndef _SYSTEM_SCENE_H
#define _SYSTEM_SCENE_H

namespace System {
    class GameObject; // Forward declaration
    class AudioEngine; // Forward declaration
    class PhysicsEngine; // Forward declaration

    class Scene {
    public:
        static GameObject* root;
        // Audio engine instance for managing audio within the scene
        static AudioEngine* audioEngine;
        // Physics engine instance for managing physics within the scene
        static PhysicsEngine* physicsEngine;

        static void Initialize();
        static void Shutdown();
    };
}

#endif // _SYSTEM_SCENE_H
