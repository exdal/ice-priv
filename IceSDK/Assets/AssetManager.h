#pragma once

#include <unordered_map>

#include "AssetFile.h"
#include "Utils/Memory.h"

#include <vector>

#include "Audio/Audio.h"
#include "Graphics/2D/Texture2D.h"

namespace IceSDK
{
	namespace Assets
	{
		class AssetManager
		{
		public:
			void Init(); // Load all asset files into memory
						 //	TODO:	Don't do that
						 //	TODO:	make it faster for low end devices!

			Memory::WeakPtr<Graphics::Texture2D> LoadTexture(const std::string &pName,
															 const Memory::WeakPtr<bx::AllocatorI> &pBxAlloc);

			Memory::Ptr<Audio::Audio> LoadAudio(const std::string &pName);

			std::string LoadText(const std::string &pName);

			std::vector<Memory::Ptr<AssetFile>> &GetAll();

		private:
			Asset Search(const std::string &pName);

			std::unordered_map<std::string, Asset> _search_cache;
			std::vector<Memory::Ptr<AssetFile>> _assets;
		};
	} // namespace Assets
} // namespace IceSDK
