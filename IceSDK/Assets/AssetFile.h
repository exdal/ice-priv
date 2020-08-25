#pragma once

#include "Asset.h"
#include "AssetHeader.h"
#include "Utils/Memory.h"

#include <vector>
#include <string>

namespace IceSDK
{
	namespace Assets
	{
		struct AssetFile
		{
			AssetHeader header;
			std::vector<uint8_t> body;

			bool IsFull() const;

			template <typename IAsset>
			IAsset Get(const std::string &pName)
			{
				return this->Get(pName).Into<IAsset>(pName);
			}
			template <typename IAsset>
			IAsset Get(const std::uint32_t pIndex)
			{
				return this->Get(pIndex).Into<IAsset>(this->header.asset_register[pIndex].name);
			}

			Asset Get(const std::string &pName);
			Asset Get(uint32_t pIndex);

			int32_t GetIndex(const std::string &pName) const;

			template <typename T>
			void Append(const std::string &pName, T pAsset)
			{
				this->Append(pName, Asset::From<T>(pAsset));
			}
			void Append(const std::string &pName, const Asset &pAsset);

			void Save(const std::string &pPath);
			static Memory::Ptr<AssetFile> Load(const std::string &pPath);

		private:
			int32_t GetEmptyEntry() const;
		};
	} // namespace Assets
} // namespace IceSDK
