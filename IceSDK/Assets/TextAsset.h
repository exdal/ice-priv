#pragma once

#include "Assets/AssetHeader.h"

#include <string>
#include <vector>

namespace IceSDK::Assets {
    class TextAsset {
    public:
        explicit TextAsset(std::string pData);

        std::string ToString() const;

        constexpr static eAssetType GetAssetType() {
            return eAssetType::Text;
        }

        uint8_t *Data() const;
        uint32_t DataSize() const;
        static TextAsset From(std::string_view _name, uint8_t *_data, const uint32_t _dataSize);

    private:
        std::string m_data;
    };
} // namespace IceSDK::Assets
