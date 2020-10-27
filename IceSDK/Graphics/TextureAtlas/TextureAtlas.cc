#include "TextureAtlas.h"
#include "GameBase.h"
#include "Graphics/TextureAtlas/Packer.h"

using namespace IceSDK::Graphics;

TextureAtlas::TextureAtlas(uint32_t textureSize, bool antiAliasing, uint32_t maxRegions) : m_regionCount(0), m_maxRegionCount(maxRegions) {
    m_packer = new Packer(textureSize, textureSize);
    m_regions = new Region[maxRegions];
    // gray scale texture: 1, RGBA8888 texture: 4, we must add gray textures -
    // TODO

    const uint32_t textureFlags = antiAliasing ? (BGFX_SAMPLER_NONE) : (BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT);

    m_texture = Texture2D::Create("Atlas Texture", textureSize, textureSize, bgfx::TextureFormat::RGBA8, textureFlags);
}

void TextureAtlas::Push(uint32_t width, uint32_t height, const uint8_t *data) {
    if (m_regionCount >= m_maxRegionCount)
        return;

    uint32_t x, y;
    if (!m_packer->Push(width, height, x, y))
        return;

    Region &reg = m_regions[m_regionCount];
    reg.x = x;
    reg.y = y;
    reg.width = width;
    reg.height = height;

    Flush(reg, data);

    m_regionCount++;
}

void TextureAtlas::Push(const std::string &path) {
    uint32_t w, h;
    uint8_t *data =
        Graphics::Texture2D::LoadRawBitmap(GetGameBase()->GetAssetManager()->GetTextureData(path), GetGameBase()->GetAssetManager()->GetTextureDataSize(path), &w, &h);
    Push(w, h, data);
}

void TextureAtlas::Flush(const Region &region, const uint8_t *data) {
    uint32_t data_size = region.width * region.height * 4;
    if (data_size) {
        // ill just update the texture like that cause i have no idea how did
        // mempler wrote that function
        const bgfx::Memory *mem = bgfx::alloc(data_size);
        bx::memCopy(mem->data, data, mem->size);
        bgfx::updateTexture2D(m_texture->GetHandle(), 0, 0, region.x, region.y, region.width, region.height, mem);
    }
}

glm::vec4 TextureAtlas::CoordinatesOf(uint32_t id) {
    if (id > m_regionCount)
        return {};

    return { m_regions[id].x, m_regions[id].y, m_regions[id].width, m_regions[id].height };
}

glm::vec2 TextureAtlas::SizeOf(uint32_t id) {
    if (id > m_regionCount)
        return {};

    return { m_regions[id].width, m_regions[id].height };
}

Memory::Ptr<Graphics::Texture2D> TextureAtlas::Texture() {
    return m_texture;
}
