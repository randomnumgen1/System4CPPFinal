#include "Vector2.hpp"
#include <string>
#include <unordered_map>
#ifndef _SYSTEM_INPUT_H
#define _SYSTEM_INPUT_H
namespace System {
    enum class KeyCode{
        None ,
        Backspace,  
        Return,//	Return key.
       A,//	'a' key.
        B,//	'b' key.
        C,//	'c' key.
        D,//	'd' key.
        E,//	'e' key.
        F,//	'f' key.
        G,//	'g' key.
        H,//	'h' key.
        I,//	'i' key.
        J,//	'j' key.
        K,//	'k' key.
        L,//	'l' key.
        M,//	'm' key.
        N,//	'n' key.
        O,//	'o' key.
        P,//	'p' key.
        Q,//	'q' key.
        R,//	'r' key.
        S,//	's' key.
        T,//	't' key.
        U,//	'u' key.
        V,//	'v' key.
        W,//	'w' key.
        X,//	'x' key.
        Y,//	'y' key.
        Z,//	'z' key.
       RightShift,//	Right shift key.
        LeftShift,//	Left shift key.
        WheelUp,//	Mouse wheel up.
        WheelDown,//	Mouse wheel down.
        Mouse0,//	The Left(or primary) mouse button.
        Mouse1,//	Right mouse button(or secondary mouse button).
        Mouse2,//	Middle mouse button(or third button).
    
    };
    class Input {
    public:
        // Store input states: true if key is currently down
    
        static Vector2 mousePos;

        static Vector2 mousePosition() {
            return mousePos;
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

 