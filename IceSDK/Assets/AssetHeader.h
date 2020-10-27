#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace IceSDK::Assets {
    constexpr auto asset_tag = 6000566310344672073;
    constexpr auto asset_version = 0x0001;
    constexpr auto asset_register_size = 256;

    enum class eFileFlags : uint32_t { NONE, ENCRYPTED = 1 << 0, COMPRESSED = 1 << 1 };

    enum class eAssetType : uint8_t {
        Unknown = 0,

        Texture = 1,
        AnimatedTexture = 2,
        Audio = 3,
        Video = 4,
        Particle = 5,

        Shader = 6,

        Text = 7
    };

#pragma pack(push, 1)
    struct AssetRegion {
        eAssetType type = eAssetType::Unknown;
        std::string name = "";
        
        uint32_t data_size = 0;
        uint8_t *data = nullptr;
    };

    struct AssetHeader {
        uint64_t tag = 0;
        uint16_t version = 0;
        eFileFlags flags = eFileFlags::NONE;
        uint8_t content_count = 0; // max 256 assets are ok
    };
#pragma pack(pop)

} // namespace IceSDK::Assets