#include "Texture2DAsset.h"

using namespace IceSDK;
using namespace IceSDK::Assets;

Texture2DAsset::Texture2DAsset(const std::string &pName, const std::vector<uint8_t> &pData)
{
	this->_name = pName;
	this->_data = pData;
}

Memory::WeakPtr<Graphics::Texture2D> Texture2DAsset::LoadTexture(const Memory::WeakPtr<bx::AllocatorI> &pBxAlloc) const
{
	return Graphics::Texture2D::Load(pBxAlloc, this->_name, this->_data);
}

eAssetType Texture2DAsset::GetAssetType()
{
	return eAssetType::Texture;
}

std::vector<uint8_t> Texture2DAsset::ToByteArray() const
{
	return this->_data;
}

Texture2DAsset Texture2DAsset::From(const std::string &pName, const std::vector<uint8_t> &pData)
{
	return Texture2DAsset{pName, pData};
}
