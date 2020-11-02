#include "Assets/AssetFile.h"
#include "Assets/AudioAsset.h"
#include "Assets/TextAsset.h"
#include "Assets/Texture2DAsset.h"

#include "Utils/FileSystem.h"
#include "Utils/Memory.h"
#include "Utils/String.h"

#include <iostream>
#undef mix // fuck yourself jenkins
#include "Utils/Logger.h"

#include "GameBase.h"

using namespace IceSDK;

const char *g_FileWhitelist[] = {
    // Audio Files
    ".ogg", ".mp3", ".wav",

    // Textures
    ".png", ".jpg", ".bmp",

    // Text Files
    ".txt"
};

IceSDK::Memory::Ptr<IceSDK::GameBase> GetGameBase() {
    return nullptr;
}

int main(const int pArgc, char *pArgv[]) {
    if (pArgc < 3) {
        std::cout << "Usage: AssetCompiler.exe "
                  << "./Path/To/Assets "
                  << "./build" << std::endl;
        return 1;
    }

    const std::string assetPath = pArgv[1];
    const std::string buildPath = pArgv[2];

    int asset_size = 0;

    if (!FileSystem::Exists(buildPath))
        FileSystem::MkDir(buildPath);

    for (const auto &dir : FileSystem::ReadDirectory(assetPath, true)) {
        if (FileSystem::IsDirectory(dir))
            continue;

        Memory::Ptr<Assets::AssetFile> asset = std::make_shared<Assets::AssetFile>();

        auto name = String::Trim(dir, "../");
        name = String::Trim(name, "..\\");
        name = String::Trim(name, "/");
        name = String::Trim(name, "\\");
        name = String::Trim(name, ".\\");
        name = String::Trim(name, "./");
        name = "/" + name;

        if (FileSystem::HasExtension(dir, ".png") || FileSystem::HasExtension(dir, ".jpg") ||
            FileSystem::HasExtension(dir, ".bmp")) { // TODO: Unpack into a specially crafted Texture format instead of using the existing ones for faster decoding

            uint32_t size = 0;
            uint8_t *data = FileSystem::ReadBinaryFile(dir, &size);
            asset->SetContent(name, Assets::Texture2DAsset::From(name, data, size));

            ICESDK_INFO_V("Asset (Texture2D): %s -> %d.ice:%s", dir.c_str(), asset_size, name.c_str());
        }

        else if (FileSystem::HasExtension(dir, ".txt")) {

            uint32_t size = 0;
            uint8_t *data = FileSystem::ReadBinaryFile(dir, &size);
            asset->SetContent(name, Assets::TextAsset::From(name, data, size));

            ICESDK_INFO_V("Asset (Text): %s -> %d.ice:%s", dir.c_str(), asset_size, name.c_str());
        }

        else if (FileSystem::HasExtension(dir, ".ogg") || FileSystem::HasExtension(dir, ".mp3") ||
                 FileSystem::HasExtension(dir, ".wav")) { // TODO: Unpack into a specially crafted Audio format instead of using the existing ones for faster decoding

            uint32_t size = 0;
            uint8_t *data = FileSystem::ReadBinaryFile(dir, &size);
            asset->SetContent(name, Assets::AudioAsset::From(name, data, size));

            ICESDK_INFO_V("Asset (Audio): %s -> %d.ice:%s", dir.c_str(), asset_size, name.c_str());
        }

        else {
            ICESDK_WARN("Unknown Asset!");
        }

        auto assetPath = FileSystem::JoinPath(buildPath, std::to_string(asset_size) + ".ice");

        ICESDK_INFO_V("Saving %s", assetPath.c_str());
        asset->Save(assetPath);
        ICESDK_INFO_V("Saved asset %s", name.c_str());
        asset_size++;
    }

    return 0;
}
