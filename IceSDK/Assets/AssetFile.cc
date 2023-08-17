#include "pch.h"

#include "Assets/AssetFile.h"

#include "Utils/FileSystem.h"
#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

Asset AssetFile::Get() {
    ICESDK_PROFILE_FUNCTION();
    return Asset{ content.type, content.data, content.data_size };
}

void AssetFile::SetContent(const std::string &name, const Asset &asset) {
    content = { asset.asset_type, name, asset.dataSize, asset.data };
}

void AssetFile::Save(const std::string &path) {
    ICESDK_PROFILE_FUNCTION();
    std::vector<uint8_t> data;
    AssetHeader header;
    header.tag = asset_tag;
    header.version = asset_version;
    Memory::serialize(header, data, sizeof(AssetHeader));
    Memory::serialize(content.type, data);
    Memory::serialize_str8(content.name, data);
    Memory::serialize(content.data_size, data);
    Memory::serialize(content.data, data, content.data_size);
    FileSystem::WriteBinaryFile(path, data.data(), data.size());
}

bool AssetFile::Load(uint8_t *mem, const uint32_t &size) {
    ICESDK_PROFILE_FUNCTION();

    if (size < sizeof(AssetHeader)) {
        // print invalid memory
        free(mem);
        return false;
    }

    uint8_t *mem_ptr = mem;

    Memory::serialize(mem, &header.tag);
    Memory::serialize(mem, &header.version);
    Memory::serialize(mem, &header.flags);

    if (header.tag != asset_tag) {
        // thats not our file
        mem = mem_ptr;
        free(mem);
        return false;
    }

    Memory::serialize(mem, &content.type);
    Memory::serialize_str8(mem, content.name);
    Memory::serialize(mem, &content.data_size);
    content.data = (uint8_t *)malloc(content.data_size);
    memcpy(content.data, mem, content.data_size);

    ICESDK_INFO_V("AssetManager: Loaded %s(type: %d, size: %d)", content.name.c_str(), content.type, content.data_size);

    mem = mem_ptr;
    free(mem);
    return true;
}

bool AssetFile::Load(const std::string &path) {
    ICESDK_PROFILE_FUNCTION();
    uint32_t size = 0;
    uint8_t *data = FileSystem::ReadBinaryFile(path, &size);
    return Load(data, size);
}