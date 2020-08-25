#pragma once

#include "AssetHeader.h"

#include <string>
#include <vector>

namespace IceSDK
{
	namespace Assets
	{
		class TextAsset
		{
		public:
			explicit TextAsset(std::string pData);

			std::string ToString() const;

			static eAssetType GetAssetType();
			std::vector<uint8_t> ToByteArray();
			static TextAsset From(std::string pName, std::vector<uint8_t> pData);

		private:
			std::string _data;
		};
	} // namespace Assets
} // namespace IceSDK
