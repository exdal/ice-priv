#include "pch.h"
#include "Assets/AssetFile.h"
#include "Utils/FileSystem.h"
#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

bool AssetFile::IsFull() const {
    return this->GetEmptyEntry() == 0;
}

Asset AssetFile::Get(const std::string &pName) {
    const auto index = GetIndex(pName);
    if (index == -1)
        return {};

    return this->Get(static_cast<uint32_t>(index));
}

Asset AssetFile::Get(const uint32_t pIndex) {
    ICESDK_PROFILE_FUNCTION();

    auto pos = 0;
    for (uint32_t i = 0; i < this->content.size(); i++) {
        if (i == pIndex)
            break;

        pos += this->content[i].data_size;
    }

    const auto region = this->content[pIndex];
    return Asset{ region.type, region.data, region.data_size };
}

int32_t AssetFile::GetIndex(const std::string &pName) const {
    ICESDK_PROFILE_FUNCTION();

    for (uint32_t i = 0; i < this->content.size(); i++) {
        if (this->content[i].name == pName)
            return i;
    }

    return -1;
}

void AssetFile::Append(const std::string &pName, const Asset &pAsset) {
    ICESDK_PROFILE_FUNCTION();
    const auto register_index = GetEmptyEntry();
    if (register_index == -1)
        return;

    this->content.push_back({ pAsset.asset_type, pName, pAsset.dataSize, pAsset.data });
}

void AssetFile::Save(const std::string &pPath) {
    ICESDK_PROFILE_FUNCTION();
    std::vector<uint8_t> data;
    AssetHeader header;
    header.tag = asset_tag;
    header.version = asset_version;
    header.content_count = this->content.size();
    Memory::serialize(header, data, sizeof(AssetHeader));
    for (const auto ct : this->content) {
        Memory::serialize(ct.type, data);
        Memory::serialize_str8(ct.name, data);
        Memory::serialize(ct.data_size, data);
        Memory::serialize(ct.data, data, ct.data_size);
    }
    
    FileSystem::WriteBinaryFile(pPath, data.data(), data.size());
}

Memory::Ptr<AssetFile> AssetFile::Load(uint8_t *mem, const uint32_t &size) {
    ICESDK_PROFILE_FUNCTION();

    if (size < sizeof(AssetHeader)) {
        // print invalid memory
        free(mem);
        return nullptr;
    }

    uint8_t *mem_ptr = mem;

    auto file = std::make_shared<AssetFile>();
    Memory::serialize(mem, &file->header.tag);
    Memory::serialize(mem, &file->header.version);
    Memory::serialize(mem, &file->header.flags);
    Memory::serialize(mem, &file->header.content_count);

    if (file->header.tag != asset_tag) {
        // thats not our file
        mem = mem_ptr;
        free(mem);
        return nullptr;
    }

    // TODO: make complete file header check with different versions

    for (size_t i = 0; i < file->header.content_count; i++) {
        AssetRegion content;
        Memory::serialize(mem, &content.type);
        Memory::serialize_str8(mem, content.name);
        Memory::serialize(mem, &content.data_size);
        content.data = (uint8_t *)malloc(content.data_size);
        memcpy(content.data, mem, content.data_size);
        mem += content.data_size;
        file->content.push_back(content);

        ICESDK_CORE_INFO("AssetManager: Loaded {}(type: {}, size: {})", content.name, content.type, content.data_size);
    }

    mem = mem_ptr;
    free(mem);
    return file;
}

Memory::Ptr<AssetFile> AssetFile::Load(const std::string &pPath) {
    ICESDK_PROFILE_FUNCTION();
    uint32_t size = 0;
    uint8_t *data = FileSystem::ReadBinaryFile(pPath, &size);
    return Load(data, size);
}

uint32_t AssetFile::GetEmptyEntry() const {
    ICESDK_PROFILE_FUNCTION();
    return (uint32_t)this->content.size();
}
