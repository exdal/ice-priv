#include <iostream>

#include "Assets/Texture2DAsset.h"
#include "Assets/AudioAsset.h"
#include "Assets/TextAsset.h"
#include "Utils/FileSystem.h"
#include "Utils/Memory.h"
#include "Utils/String.h"

using namespace IceSDK;

const char *g_FileWhitelist[] = {
	// Audio Files
	".ogg", ".mp3", ".wav",

	// Textures
	".png", ".jpg", ".bmp",

	// Text Files
	".txt"};

IceSDK::Memory::Ptr<IceSDK::GameBase> GetGameBase()
{
	return nullptr;
}

int main(const int pArgc, char *pArgv[])
{
	if (pArgc < 3)
	{
		std::cout << "Usage: AssetCompiler.exe "
				  << "./Path/To/Assets"
				  << "./build" << std::endl;
		return 1;
	}

	const std::string assetPath = pArgv[1];
	const std::string buildPath = pArgv[2];

	std::vector<Memory::Ptr<Assets::AssetFile>> assets;

	for (const auto &dir : FileSystem::ReadDirectory(assetPath, true))
	{
		if (FileSystem::IsDirectory(dir))
			continue;

		Memory::Ptr<Assets::AssetFile> free_asset = nullptr;
		for (const auto asset : assets) // NOTE: this will get slower and slower the more assets we have!
		{
			if (!asset->IsFull())
				free_asset = asset;
		}

		if (free_asset == nullptr)
		{
			free_asset = std::make_shared<Assets::AssetFile>();

			assets.push_back(free_asset);
		}

		auto name = String::Trim(dir, "../");
		name = String::Trim(name, "..\\");
		name = String::Trim(name, "/");
		name = String::Trim(name, "\\");
		name = String::Trim(name, ".\\");
		name = String::Trim(name, "./");
		name = "/" + name;

		if (FileSystem::HasExtension(dir, ".txt"))
		{
			free_asset->Append(name, Assets::TextAsset::From(name, FileSystem::ReadBinaryFile(dir)));
		}

		if (FileSystem::HasExtension(dir, ".png") ||
			FileSystem::HasExtension(dir, ".jpg") ||
			FileSystem::HasExtension(dir, ".bmp"))
		{
			free_asset->Append(name, Assets::Texture2DAsset::From(name, FileSystem::ReadBinaryFile(dir)));
		}

		if (FileSystem::HasExtension(dir, ".ogg") ||
			FileSystem::HasExtension(dir, ".mp3") ||
			FileSystem::HasExtension(dir, ".wav"))
		{
			free_asset->Append(name, Assets::AudioAsset::From(name, FileSystem::ReadBinaryFile(dir)));
		}

		std::cout << "Asset: " << dir << " Key: " << name << std::endl;
	}

	FileSystem::MkDir(buildPath);

	for (auto i = 0; i < assets.size(); ++i)
	{
		auto asset = assets[i];

		asset->Save(FileSystem::JoinPath(buildPath, std::to_string(i) + ".pxl"));
	}

	return 0;
}
