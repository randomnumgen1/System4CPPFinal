#include <System/Component.hpp>
#include <System/GameObject.hpp>

namespace System {
    bool Component::CompareTag(std::string tag) const {
        return tag == m_gameObject->tag;
    }
}
