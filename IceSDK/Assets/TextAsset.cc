#include "pch.h"
#include "Assets/TextAsset.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

TextAsset::TextAsset(uint8_t *_data, const uint32_t _dataSize) : m_rawData(_data), m_dataSize(_dataSize) {
    m_data = std::string((const char *)_data, _dataSize);
}

std::string TextAsset::ToString() const {
    return m_data;
}

uint8_t *TextAsset::Data() const {
    return m_rawData;
}

uint32_t TextAsset::DataSize() const {
    return m_data.length();
}

TextAsset TextAsset::From(std::string_view _name, uint8_t *_data, const uint32_t _dataSize) {
    return TextAsset{ _data, _dataSize };
}
