#ifndef _SYSTEM_COMPONENT_H
#define _SYSTEM_COMPONENT_H


namespace System {
    class GameObject;
    class Transform;
}

namespace System {
    struct Component {
    private:
        friend class GameObject;
        GameObject* gameObject = nullptr;
        Transform* transform = nullptr;
    public:
        Transform transform() {
        
        }

    };



};
#endif