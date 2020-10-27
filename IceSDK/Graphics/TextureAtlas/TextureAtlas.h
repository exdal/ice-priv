#pragma once
#include "Graphics/Texture2D.h"

#include <bgfx/bgfx.h>

class Packer;

namespace IceSDK::Graphics {
    struct Region {
        uint32_t x, y, width, height;
    };

    class TextureAtlas {
    public:
        TextureAtlas(uint32_t textureSize, bool antiAliasing, uint32_t maxRegions = 0x1000);
        void Push(uint32_t width, uint32_t height, const uint8_t *data);
        void Push(const std::string &path);

        Memory::Ptr<Graphics::Texture2D> Texture();
        glm::vec4 CoordinatesOf(uint32_t id);
        glm::vec2 SizeOf(uint32_t id);

    private:
        void Flush(const Region &region, const uint8_t *data);

        Packer *m_packer;
        Region *m_regions;
        Memory::Ptr<Graphics::Texture2D> m_texture;
        uint32_t m_textures;

        uint16_t m_regionCount;
        uint16_t m_maxRegionCount;
    };
} // namespace IceSDK::Graphics