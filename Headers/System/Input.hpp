#include "Vector2.hpp"
#include <string>
#include <unordered_map>
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
        Mouse2
    };

    class Input {
    public:
        // Store input states: true if key is currently down
    
        static Vector2 mousePos;

        static Vector2 mousePosition() {
            return mousePos;
        } 
  
        static float GetAxis(std::string axis){
            if (axis == "Horizontal") {

            }else if (axis == "Vertical") {
            
            }else{
                throw std::invalid_argument("Axis not recognised");
            }
        }
        static bool GetKey(KeyCode key) {
            
        }

        static bool GetKeyDown(KeyCode key) {
        
        }

        static bool GetKeyUp(KeyCode key) {
            
        }
        static void UpdateKeyState(uint32_t vkCode, bool isDown) {
     
        }

        static void UpdateMousePosition(Vector2 pos) {
            mousePos = pos;
        }
        // Call once per frame to update the key states
        static void UpdateInputState(){
        
        }
    };

  
}
#endif

 