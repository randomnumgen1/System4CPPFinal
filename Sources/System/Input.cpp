#include <System/Input.hpp>

namespace System {
    std::unordered_map<KeyCode, bool> Input::currentKeys;
    std::unordered_map<KeyCode, bool> Input::previousKeys;
    Vector2 Input::mousePos = Vector2(0, 0);
    Vector2 Input::lastMousePos = Vector2(0, 0);
    float Input::rawDeltaX = 0.0f;
    float Input::rawDeltaY = 0.0f;
}
