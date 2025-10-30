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
        GameObject* m_gameObject = nullptr;
        Transform* m_transform = nullptr;
    public:
        Transform* transform() {
            return m_transform;
        }
        GameObject* gameObject() {
            return m_gameObject;
        }
        bool CompareTag(std::string tag) const{
            return tag == m_gameObject->tag;
        }
        virtual Component* Clone() const = 0;

        virtual void Awake(){}
        virtual void Start(){}
        virtual void Update(){}
        virtual void FixedUpdate(){}
        virtual void LateUpdate(){}
        
        virtual void OnTriggerEnter(){}
        virtual void OnTriggerStay(){}
        virtual void OnTriggerExit(){}

        virtual void OnCollisionEnter(){}
        virtual void OnCollisionStay(){}
        virtual void OnCollisionExit(){}




    };



};
#endif