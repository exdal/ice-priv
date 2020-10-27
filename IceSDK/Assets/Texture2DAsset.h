#pragma once
#include "Assets/Asset.h"
#include "Utils/Memory.h"
#include "Graphics/Texture2D.h"
#include <cstdint>

namespace IceSDK::Assets {
    class Texture2DAsset {
    public:
        explicit Texture2DAsset(std::string_view _name, uint8_t *_data, const uint32_t _dataSize);

        Memory::Ptr<Graphics::Texture2D> LoadTexture() const;

        constexpr static eAssetType GetAssetType() {
            return eAssetType::Texture;
        }

        uint8_t *Data() const;
        uint32_t DataSize() const;
        static Texture2DAsset From(std::string_view _name, uint8_t *_data, const uint32_t _dataSize);

    private:
        std::string m_name;
        uint8_t *m_data;
        uint32_t m_dataSize;
    };
} // namespace IceSDK::Assets
