#include "pch.h"

#include "Input/InputPipeline.h"

#include "Utils/Logger.h"

#include "GameBase.h"

using namespace IceSDK;
using namespace IceSDK::Input;

//static InputPipeline* g_InputPipeline;

#if defined(ICESDK_GLFW)
void InputPipeline::MouseButtonCallback(GLFWwindow *pWnd, int pButton, int pButtonState, int pMods) {
    auto inputPipeline = GetGameBase()->GetInputPipeline();

    inputPipeline->_mouse_state.insert_or_assign((MouseTable)pButton, (ButtonState)pButtonState);
}

void InputPipeline::CursorCallback(GLFWwindow *pWnd, double pX, double pY) {
    auto inputPipeline = GetGameBase()->GetInputPipeline();

    inputPipeline->_mouse_move_delta.x = pX;
    inputPipeline->_mouse_move_delta.y = pY;
}

void InputPipeline::ScrollCallback(GLFWwindow *pWnd, double pX, double pY) {
    auto inputPipeline = GetGameBase()->GetInputPipeline();

    inputPipeline->_mouse_scroll_axis.x = pX;
    inputPipeline->_mouse_scroll_axis.y = pY;
}

void InputPipeline::KeybrdCallback(GLFWwindow *pWnd, int pKey, int pScanCode, int pButtonState, int pMods) {
    auto inputPipeline = GetGameBase()->GetInputPipeline();

    inputPipeline->_keyboard_mods = (KeyboardMods)pMods;
    inputPipeline->_keyboard_state.insert_or_assign((KeyboardTable)pKey, (ButtonState)pButtonState);
}
#elif defined(ICESDK_SDL2)
void InputPipeline::PumpSDL2Event(SDL_Event &pEvent) {
    if (pEvent.type == SDL_KEYDOWN || pEvent.type == SDL_KEYUP) {
        auto keyEvent = pEvent.key.keysym;

        uint8_t keyboardMods = KeyboardMods::None;

        if ((keyEvent.mod & KMOD_LSHIFT) > 0) {
            keyboardMods |= KeyboardMods::SHIFT;
        }

        if ((keyEvent.mod & KMOD_RSHIFT) > 0) {
            keyboardMods |= KeyboardMods::SHIFT;
        }

        if ((keyEvent.mod & KMOD_LCTRL) > 0) {
            keyboardMods |= KeyboardMods::CONTROL;
        }

        if ((keyEvent.mod & KMOD_RCTRL) > 0) {
            keyboardMods |= KeyboardMods::CONTROL;
        }

        if ((keyEvent.mod & KMOD_RALT) > 0) {
            keyboardMods |= KeyboardMods::ALT;
        }

        if ((keyEvent.mod & KMOD_LALT) > 0) {
            keyboardMods |= KeyboardMods::ALT;
        }

        if ((keyEvent.mod & KMOD_LGUI) > 0) {
            keyboardMods |= KeyboardMods::SUPER;
        }

        if ((keyEvent.mod & KMOD_RGUI) > 0) {
            keyboardMods |= KeyboardMods::SUPER;
        }

        if ((keyEvent.mod & KMOD_NUM) > 0) {
            keyboardMods |= KeyboardMods::NUM_LOCK;
        }

        if ((keyEvent.mod & KMOD_CAPS) > 0) {
            keyboardMods |= KeyboardMods::CAPS_LOCK;
        }

        this->_keyboard_mods = (KeyboardMods)keyboardMods;
    }

    if (pEvent.type == SDL_KEYDOWN) {
        auto keyEvent = pEvent.key.keysym;

        this->_keyboard_state.insert_or_assign((KeyboardTable)SDL_SCANCODE_TO_KEYCODE(keyEvent.scancode), ButtonState::Pressed);
    }

    if (pEvent.type == SDL_KEYUP) {
        auto keyEvent = pEvent.key.keysym;

        this->_keyboard_state.insert_or_assign((KeyboardTable)SDL_SCANCODE_TO_KEYCODE(keyEvent.scancode), ButtonState::Released);
    }

    if (pEvent.type == SDL_MOUSEMOTION) {
        auto motionEvent = pEvent.motion;

        this->_mouse_move_delta.x = motionEvent.x;
        this->_mouse_move_delta.y = motionEvent.y;
    }

    if (pEvent.type == SDL_MOUSEWHEEL) {
        auto scrollEvent = pEvent.wheel;

        this->_mouse_scroll_axis.x = scrollEvent.x;
        this->_mouse_scroll_axis.y = scrollEvent.y;
    }

    if (pEvent.type == SDL_MOUSEBUTTONDOWN) {
        auto mouseButtonEvent = pEvent.button;

        this->_mouse_state.insert_or_assign((MouseTable)(mouseButtonEvent.button + 1), ButtonState::Pressed);
    }

    if (pEvent.type == SDL_MOUSEBUTTONUP) {
        auto mouseButtonEvent = pEvent.button;

        this->_mouse_state.insert_or_assign((MouseTable)(mouseButtonEvent.button + 1), ButtonState::Released);
    }
}
#endif

void InputPipeline::Init() {
#if defined(ICESDK_GLFW)
    auto nativeWindow = GetWindow()->GetNativeWindow();

    glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetMouseButtonCallback(nativeWindow, InputPipeline::MouseButtonCallback);
    glfwSetCursorPosCallback(nativeWindow, InputPipeline::CursorCallback);
    glfwSetScrollCallback(nativeWindow, InputPipeline::ScrollCallback);
    glfwSetKeyCallback(nativeWindow, InputPipeline::KeybrdCallback);
#endif
}
