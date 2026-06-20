#include "Vector2.hpp"
#include <string>
#include <unordered_map>

#if _WIN32
#include <windows.h>   // for VK_* codes
#endif

#ifndef _SYSTEM_INPUT_H
#define _SYSTEM_INPUT_H

namespace System {

    enum class KeyCode {
        None,
        Backspace,
        Return,
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        RightShift,
        LeftShift,
        WheelUp,
        WheelDown,
        Mouse0,
        Mouse1,
        Mouse2,

        UpArrow,
        DownArrow,
        LeftArrow,
        RightArrow
    };

    class Input {
    private:
        static std::unordered_map<KeyCode, bool> currentKeys;
        static std::unordered_map<KeyCode, bool> previousKeys;
        static Vector2 mousePos;
        static Vector2 lastMousePos;

        // NEW raw mouse deltas
        static float rawDeltaX;
        static float rawDeltaY;


    public: 

        static Vector2 mousePosition() {
            return mousePos;
        }
        // --- RAW MOUSE ACCESS ---
        static float GetRawMouseDeltaX() { return rawDeltaX; }
        static float GetRawMouseDeltaY() { return rawDeltaY; }

        static float GetAxis(const std::string& axis) {
            if (axis == "MouseX") {
                return mousePos.x - lastMousePos.x;
            }
            else if (axis == "MouseY") {
                return mousePos.y - lastMousePos.y;
            }
            else {
                throw std::invalid_argument("Axis not recognised");
            }
        }


        static bool GetKey(KeyCode key) {
            return currentKeys[key];
        }

        static bool GetKeyDown(KeyCode key) {
            return currentKeys[key] && !previousKeys[key];
        }

        static bool GetKeyUp(KeyCode key) {
            return !currentKeys[key] && previousKeys[key];
        }
        // Called from WndProc  WM_INPUT
        static void UpdateRawMouseDelta(LONG dx, LONG dy) {
            rawDeltaX += (float)dx;
            rawDeltaY += (float)dy;
        }
        // Optional: still allow cursor based position
        static void UpdateMousePosition(Vector2 pos) {
            mousePos = pos;
        }

        // Called from WndProc when a key event occurs
        static void UpdateKeyState(uint32_t vkCode, bool isDown) {
            KeyCode key = TranslateVK(vkCode);
            if (key != KeyCode::None) {
                currentKeys[key] = isDown;
            }
        }

  

        // Call once per frame to update the key states
        static void UpdateInputState() {
            previousKeys = currentKeys;

            // reset raw deltas each frame
            rawDeltaX = 0.0f;
            rawDeltaY = 0.0f;


            lastMousePos = mousePos;

        }

    private:
        // Map Windows VK codes to your KeyCode enum
        static KeyCode TranslateVK(uint32_t vk) {
            switch (vk) {
            case VK_BACK: return KeyCode::Backspace;
            case VK_RETURN: return KeyCode::Return;
        

            case 'A': return KeyCode::A;
            case 'B': return KeyCode::B;
            case 'C': return KeyCode::C;
            case 'D': return KeyCode::D;
            case 'E': return KeyCode::E;
            case 'F': return KeyCode::F;
            case 'G': return KeyCode::G;
            case 'H': return KeyCode::H;
            case 'I': return KeyCode::I;
            case 'J': return KeyCode::J;
            case 'K': return KeyCode::K;
            case 'L': return KeyCode::L;
            case 'M': return KeyCode::M;
            case 'N': return KeyCode::N;
            case 'O': return KeyCode::O;
            case 'P': return KeyCode::P;
            case 'Q': return KeyCode::Q;
            case 'R': return KeyCode::R;
            case 'S': return KeyCode::S;
            case 'T': return KeyCode::T;
            case 'U': return KeyCode::U;
            case 'V': return KeyCode::V;
            case 'W': return KeyCode::W;
            case 'X': return KeyCode::X;
            case 'Y': return KeyCode::Y;
            case 'Z': return KeyCode::Z;



                //UpArrow



            case VK_UP: return KeyCode::UpArrow;
            case VK_DOWN: return KeyCode::DownArrow;
            case VK_LEFT: return KeyCode::LeftArrow;
            case VK_RIGHT: return KeyCode::RightArrow;

           
            case VK_SHIFT: return KeyCode::LeftShift; // refine if needed
            case VK_LBUTTON: return KeyCode::Mouse0;
            case VK_RBUTTON: return KeyCode::Mouse1;
            case VK_MBUTTON: return KeyCode::Mouse2;
            default: return KeyCode::None;
            }
        }
    };

    // Static member definitions
    std::unordered_map<KeyCode, bool> Input::currentKeys;
    std::unordered_map<KeyCode, bool> Input::previousKeys;
    Vector2 Input::mousePos = Vector2(0, 0);
    Vector2 Input::lastMousePos = Vector2(0, 0);
    float Input::rawDeltaX = 0.0f;
    float Input::rawDeltaY = 0.0f;

}

#endif