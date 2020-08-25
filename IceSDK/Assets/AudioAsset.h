#pragma once
#include <vector>

#include "AssetHeader.h"
#include "Audio/AudioSystem.h"
#include "GameBase.h"

namespace IceSDK
{
	namespace Assets
	{
		class AudioAsset
		{
		public:
			explicit AudioAsset(std::vector<uint8_t> pData);

			Memory::Ptr<Audio::Audio> LoadAudio() const;

			static eAssetType GetAssetType();

			std::vector<uint8_t> ToByteArray() const;
			static AudioAsset From(std::string pName, std::vector<uint8_t> pData);

		private:
			std::vector<uint8_t> _data{};
		};
	} // namespace Assets
} // namespace IceSDK
