#pragma once

#include "Utils/Memory.h"

#include "Graphics/Components/AnimationComponent.h"
#include "Graphics/Texture2D.h"

#include <glm/glm.hpp>

#include <vector>

namespace IceSDK::Graphics {
    enum class TileFlags : uint32_t { NONE, ANIMATED = 1 << 0, SPECIAL = 1 << 1 };

    struct Tile {
        uint32_t id{};
        TileFlags flags;
        glm::vec2 coordinates;

        union { // ANIMATED TILE
            glm::vec2 frames{};
            float delay;
        };

        union { // SPECIAL TILE
            glm::vec2 size{};
        };
    };

    class Tilesheet {
    public:
        explicit Tilesheet(Memory::Ptr<Texture2D> texture, const char *infoPath);
        ~Tilesheet();

        Tile GetTile(uint32_t ID);
        Memory::Ptr<Texture2D> GetTexture();
        uint32_t TileSize();

    private:
        Memory::Ptr<Texture2D> m_texture;
        std::vector<Tile> m_tiles;
    };
} // namespace IceSDK::Graphics