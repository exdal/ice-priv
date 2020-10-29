#include "pch.h"
#include "Assets/Texture2DAsset.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

Texture2DAsset::Texture2DAsset(std::string_view _name, uint8_t *_data, const uint32_t _dataSize) : m_name(_name), m_data(_data), m_dataSize(_dataSize) {
}

Memory::Ptr<Graphics::Texture2D> Texture2DAsset::LoadTexture(bool antiAliasing) const {
    return Graphics::Texture2D::Load(m_name, antiAliasing, m_data, m_dataSize);
}

uint8_t *Texture2DAsset::Data() const {
    return m_data;
}

uint32_t Texture2DAsset::DataSize() const {
    return m_dataSize;
}

Texture2DAsset Texture2DAsset::From(std::string_view _name, uint8_t *_data, const uint32_t _dataSize) {
    Texture2DAsset asset{ _name, _data, _dataSize };
    return asset;
}
