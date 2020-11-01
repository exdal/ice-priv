#pragma once

#include <glm/vec2.hpp>
#include <vector>

namespace IceSDK::Graphics::Components {
    struct AnimationComponent { // basic animation component
        std::vector<glm::vec2> frames;
        double delay;
        double last_frame;
        uint32_t current_frame;
    };
} // namespace IceSDK::Graphics::Components
