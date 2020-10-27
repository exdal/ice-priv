#include "pch.h"

#include "Assets/AudioAsset.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

Memory::Ptr<Audio::AudioSystem> GetAudioSystem();

AudioAsset::AudioAsset(uint8_t *_data, const uint32_t _dataSize) : m_data(_data), m_dataSize(_dataSize) {
}

Memory::Ptr<Audio::Sound> AudioAsset::LoadAudio() const {
    return GetAudioSystem()->Load(m_data, m_dataSize);
}

uint8_t *AudioAsset::Data() const {
    return m_data;
}

uint32_t AudioAsset::DataSize() const {
    return m_dataSize;
}

AudioAsset AudioAsset::From(std::string_view _name, uint8_t *_data, const uint32_t _dataSize) {
    return AudioAsset{ _data, _dataSize };
}
