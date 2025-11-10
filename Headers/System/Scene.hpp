#ifndef _SYSTEM_SCENE_H
#define _SYSTEM_SCENE_H

namespace System {
    class GameObject; // Forward declaration
    class AudioEngineInterface; // Forward declaration
    class PhysicsEngineInterface; // Forward declaration

    class Scene {
    public:
        static GameObject* root;
        // Audio engine instance for managing audio within the scene
        static AudioEngineInterface* audioEngine;
        // Physics engine instance for managing physics within the scene
        static PhysicsEngineInterface* physicsEngine;

        static void Initialize();
        static void Shutdown();
    };
}

#endif // _SYSTEM_SCENE_H
