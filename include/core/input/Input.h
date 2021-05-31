#pragma once
#include <unordered_map>
#include "Key.h"
#include "KeyState.h"
#include "ButtonState.h"
#include "GLFW/glfw3.h"

namespace se
{
   //class Input
   namespace Input
    {
//    //private:
//        std::unordered_map<Key, KeyState> keymap;
//        int mouse_x{};
//        int mouse_y{};

    //protected:
        static auto toSEKey(int keycode)->Key;
        static auto toButtonState(int state)->ButtonState;
        auto saveKeymap(Key key, ButtonState state) -> void;
        auto saveKeymap(int keycode, int state) -> void;
        auto saveMouseButtonToKeymap(int state) -> void;

    //public:
        // Input();
        auto initInput() -> void;
        auto checkKey(KeyState state, Key key) -> bool;
        template<typename... Keys>
        auto checkKey(KeyState state, Key key, Keys ...keys) -> bool;
        auto getMousePosition()->std::pair<int, int>;
        auto key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)->void;
        auto mouse_button_callback(GLFWwindow* window, int button, int action, int mods)->void;
        auto cursor_position_callback(GLFWwindow* window, double xpos, double ypos)->void;

    //private:
        [[deprecated("이제 사용하지 않음")]]
        auto pollKeyEvent() -> void;
    };
}