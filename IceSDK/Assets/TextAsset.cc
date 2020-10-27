#include "pch.h"

#include "Assets/TextAsset.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

TextAsset::TextAsset(std::string pData) : m_data(pData) {
}

std::string TextAsset::ToString() const {
    return m_data;
}

uint8_t *TextAsset::Data() const {
    return (uint8_t *)m_data.c_str();
}

uint32_t TextAsset::DataSize() const {
    return m_data.length();
}

TextAsset TextAsset::From(std::string_view _name, uint8_t *_data, const uint32_t _dataSize) {
    return TextAsset{ std::string((const char *)_data, _dataSize) };
}
