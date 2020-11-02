#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetHeader.h"

#include "Utils/Memory.h"

#include <string>
#include <vector>

namespace IceSDK::Assets {
    struct AssetFile {
        AssetHeader header;
        AssetRegion content;

        template <typename IAsset>
        IAsset Get() {
            return Get().Into<IAsset>(m_content.name);
        }

        Asset Get();

        template <typename T>
        void SetContent(const std::string &name, T asset) {
            SetContent(name, Asset::From<T>(asset));
        }
        void SetContent(const std::string &name, const Asset &asset);
        void Save(const std::string &pPath);
        bool Load(const std::string &path);
        bool Load(uint8_t *mem, const uint32_t &size);
    };
} // namespace IceSDK::Assets
