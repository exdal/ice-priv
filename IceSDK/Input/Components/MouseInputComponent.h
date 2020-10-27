#pragma once

#include <unordered_map>

#include "Input/ButtonState.h"
#include "Input/MouseTable.h"

#include <glm/vec2.hpp>

namespace IceSDK::Input::Components {
    struct MouseInputComponent {
        glm::vec2 ScrollAxis;
        glm::vec2 Position;

        std::unordered_map<MouseTable, ButtonState> MouseButtonState;

        bool IsPressed(MouseTable pBtn) {
            return this->MouseButtonState.at(pBtn) == ButtonState::Pressed;
        }
        bool IsReleased(MouseTable pBtn) {
            return this->MouseButtonState.at(pBtn) == ButtonState::Released;
        }
    };
} // namespace IceSDK::Input::Components
