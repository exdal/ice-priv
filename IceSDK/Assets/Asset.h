#pragma once

#include "Assets/AssetHeader.h"

#include <vector>
#include <cstdint>
#include <string>

namespace IceSDK
{
	namespace Assets
	{
		struct Asset
		{
			eAssetType asset_type = eAssetType::Unknown;
			std::vector<uint8_t> data{};

			template <typename IAsset>
			IAsset Into(const std::string &pName)
			{
				return IAsset::From(pName, this->data);
			}

			template <typename IAsset>
			static Asset From(IAsset pTo)
			{
				Asset asset;

				asset.asset_type = IAsset::GetAssetType();
				asset.data = pTo.ToByteArray();

				return asset;
			}
		};
	} // namespace Assets
} // namespace IceSDK
