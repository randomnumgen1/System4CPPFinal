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

        /// <summary>
        /// Initializes the scene, setting up audio and physics engines.
        /// </summary>
        static void Initialize();
        /// <summary>
        /// this is a individual scene run, called every frame from the main loop.
        /// This doesn't update Time related values, that is done in the main loop.
        /// </summary>
        /// <param name="windowWidth"></param>
        /// <param name="windowHeight"></param>
        static void Run(int windowWidth, int windowHeight);
        
        static void Shutdown();
    };
}

#endif // _SYSTEM_SCENE_H
