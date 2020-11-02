#pragma once

#include "Assets/AssetHeader.h"

#include <cstdint>
#include <string>
#include <vector>

namespace IceSDK::Assets {
    struct Asset {
        Asset() {
        }

        Asset(eAssetType eType, uint8_t *_data, const uint32_t _dataSize) : asset_type(eType), data(_data), dataSize(_dataSize) {
        }

        eAssetType asset_type = eAssetType::Unknown;
        uint8_t *data = nullptr;
        uint32_t dataSize = 0;

        template <typename IAsset>
        IAsset Into(const std::string &name) {
            return IAsset::From(name, data, dataSize);
        }

        template <typename IAsset>
        static Asset From(IAsset to) {
            Asset asset;

            asset.asset_type = IAsset::GetAssetType();
            asset.data = to.Data();
            asset.dataSize = to.DataSize();

            return asset;
        }
    };
} // namespace IceSDK::Assets
