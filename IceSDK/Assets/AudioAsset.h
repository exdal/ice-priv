#pragma once

#include "Assets/AssetHeader.h"
#include "Audio/AudioSystem.h"

#include <vector>

namespace IceSDK::Assets {
    class AudioAsset {
    public:
        explicit AudioAsset(uint8_t *_data, const uint32_t _dataSize);

        Memory::Ptr<Audio::Sound> LoadAudio() const;

        constexpr static eAssetType GetAssetType() {
            return eAssetType::Audio;
        }

        uint8_t *Data() const;
        uint32_t DataSize() const;
        static AudioAsset From(std::string_view _name, uint8_t *_data, const uint32_t _dataSize);

    private:
        uint8_t *m_data;
        uint32_t m_dataSize;
    };
} // namespace IceSDK::Assets
